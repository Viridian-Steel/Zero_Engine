#include "First_App.hpp"

#include "Simple_Render_System.hpp"
#include "Keyboard_Movement_Controller.hpp"
#include "Z_Camera.hpp"
#include "Z_Buffer.hpp"
//glm lib
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>
#include <cassert>
#include <stdexcept>
#include <array>


namespace ze {

	struct GlobalUBO {
		glm::mat4 projectionView{ 1.f };
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
		glm::vec3 lightPosition{ -1.f };
		alignas(16) glm::vec4 LightColor{ 1.f };
	};


	FirstApp::FirstApp()
	{
		globalPool = ZDescriptorPool::Builder(zDevice)
			.setMaxSets(ZSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, ZSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		loadGameObjects();
	}
	FirstApp::~FirstApp()
	{
		
	}
	void FirstApp::run()
	{
		std::vector<std::unique_ptr<ZBuffer>> uboBuffers(ZSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++) {
			uboBuffers[i] = std::make_unique<ZBuffer>(
				zDevice,
				sizeof(GlobalUBO),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			uboBuffers[i]->map();
		}

		auto globalSetLayout = ZDescriptorSetLayout::Builder(zDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(ZSwapChain::MAX_FRAMES_IN_FLIGHT);

		for (int i = 0; i < globalDescriptorSets.size(); i++) {
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			ZDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem{zDevice, zRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        ZCamera camera{};

        auto viewerObject = ZGameObject::createGameObject();

		viewerObject.transform.translation.z = -2.5f;
        KeyBoardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!zWindow.shouldClose()) {
			glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveinPlaneXZ(zWindow.getGlfwWindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = zRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

			if (auto commandBuffer = zRenderer.beginFrame()) {
				int frameIndex = zRenderer.getFrameIndex();

				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					commandBuffer,
					camera,
					globalDescriptorSets[frameIndex],
					gameObjects
				};
					
				//update
				GlobalUBO ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();

				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				//render
				zRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo);
				zRenderer.endSwapChainRenderPass(commandBuffer);
				zRenderer.endFrame();
			}
		}
	}

	void FirstApp::loadGameObjects()
	{
        std::shared_ptr<ZModel> zModel = ZModel::creatModelFromFile(zDevice, "models/flat_vase.obj");

        auto flatVase = ZGameObject::createGameObject();
        flatVase.model = zModel;
        flatVase.transform.translation = { -.5f, 0.5f, 0.f };
		flatVase.transform.scale = glm::vec3{ 3.f };

        gameObjects.emplace(flatVase.getId(), std::move(flatVase));

		zModel = ZModel::creatModelFromFile(zDevice, "models/smooth_vase.obj");

		auto smoothVase = ZGameObject::createGameObject();
		smoothVase.model = zModel;
		smoothVase.transform.translation = { .5f, 0.5f, 0.f};
		smoothVase.transform.scale = glm::vec3{ 3.f };

		gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

		zModel = ZModel::creatModelFromFile(zDevice, "models/quad.obj");

		auto floor = ZGameObject::createGameObject();
		floor.model = zModel;
		floor.transform.translation = { 0.f, 0.5f, 0.f};
		floor.transform.scale = glm::vec3{ 3.f, 1.f, 3.f };

		gameObjects.emplace(floor.getId(), std::move(floor));
	}
}

