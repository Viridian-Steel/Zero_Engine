#include "First_App.hpp"
#include "Simple_Render_System.hpp"

//glm lib
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cassert>
#include <stdexcept>
#include <array>


namespace ze {


	FirstApp::FirstApp()
	{
		loadGameObjects();
	}
	FirstApp::~FirstApp()
	{
		
	}
	void FirstApp::run()
	{
		SimpleRenderSystem simpleRenderSystem{zDevice, zRenderer.getSwapChainRenderPass()};
		while (!zWindow.shouldClose()) {
			glfwPollEvents();
			
			if (auto commandBuffer = zRenderer.beginFrame()) {
				zRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				zRenderer.endSwapChainRenderPass(commandBuffer);
				zRenderer.endFrame();
			}
		}
	}

	void FirstApp::loadGameObjects()
	{
		std::vector<ZModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.f, 0.f, 0.f}},
			{{0.5f, 0.5f}, {0.f, 1.f, 0.f}},
			{{-0.5f, 0.5f}, {0.f, 0.f, 1.f}}
		};
		auto zModel = std::make_shared<ZModel>(zDevice, vertices);

		auto triangle = ZGameObject::createGameObject();

		triangle.model = zModel;
		triangle.color = { .1f, .8f, .1f };

		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = { 2.f, 0.5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}
}

