#include "PointLightSystem.hpp"

//glm lib
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cassert>
#include <stdexcept>
#include <array>

#ifndef ENGINE_DIR
#define ENGINE_DIR "../"
#endif

namespace ze {
	
	struct PointLightPushConstants{
		glm::vec4 position{};
		glm::vec4 color{};
		float radius;
	};

	PointLightSystem::PointLightSystem(ZDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : zDevice{ device }
	{
		createPipelineLayout(globalSetLayout);
		createPipeline(renderPass);
	}
	PointLightSystem::~PointLightSystem()
	{
		vkDestroyPipelineLayout(zDevice.device(), pipelineLayout, nullptr);
	}

	void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PointLightPushConstants);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(zDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout.");
		}
	}

	void PointLightSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(pipelineLayout != nullptr && "cannot create Pipeline before Pipeline Layout");

		PipelineConfigInfo pipelineConfig{};
		ZePipeline::defaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.attributeDescriptions.clear();
		pipelineConfig.bindingDescriptions.clear();

		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;

		zPipeline = std::make_unique<ZePipeline>(
			zDevice,
			"Shaders/point_light.vert.spv",
			"Shaders/point_light.frag.spv",
			pipelineConfig
			);

	}

	void PointLightSystem::update(FrameInfo& frameInfo, GlobalUBO& ubo){
		auto rotateLight = glm::rotate(glm::mat4(1.f), frameInfo.frameTime,{ 0.f, -1.f, 0.f});
		int lightIndex = 0;
		for(auto& kv: frameInfo.gameObjects){
			auto&obj = kv.second;
			if(obj.pointLightComponent == nullptr) continue;
			
			assert(lightIndex < MAX_LIGHTS && "Point Lights Exceeded max limit");
			obj.transform.translation = glm::vec3(rotateLight * glm::vec4(obj.transform.translation, 1.f));

			ubo.pointLights[lightIndex].position = glm::vec4(obj.transform.translation, 1.f);
			ubo.pointLights[lightIndex].color = glm::vec4(obj.color, obj.pointLightComponent->lightIntensity);
			lightIndex++;
		}
		ubo.numLights = lightIndex;
	}

	void PointLightSystem::render(FrameInfo& frameInfo) {

		zPipeline->bind(frameInfo.commandBuffer);

		vkCmdBindDescriptorSets(
			frameInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout,
			0,
			1,
			&frameInfo.globalDescriptorSet,
			0,
			nullptr
		);

		for(auto& kv: frameInfo.gameObjects){
			auto&obj = kv.second;
			if(obj.pointLightComponent == nullptr) continue;
			PointLightPushConstants push{};
			push.position = glm::vec4(obj.transform.translation, 1.f);
			push.color = glm::vec4(obj.color, obj.pointLightComponent->lightIntensity);
			push.radius = obj.transform.scale.x;

			vkCmdPushConstants(
				frameInfo.commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(PointLightPushConstants),
				&push
			);
			vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
		}

	}
}

