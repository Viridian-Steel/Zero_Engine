#pragma once

#include "Z_Camera.hpp"
#include "Z_GameObject.hpp"

#include <vulkan/vulkan.h>

namespace ze {
	#define MAX_LIGHTS 10

	struct PointLight{
		glm::vec4 position{}; //ignore w
		glm::vec4 color{}; //w = instensity
	};

	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		ZCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		ZGameObject::Map& gameObjects;
	};
	struct GlobalUBO {
		glm::mat4 projection{ 1.f };
		glm::mat4 view{ 1.f };
		glm::mat4 invView{ 1.f };
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
		PointLight pointLights[MAX_LIGHTS];
		int numLights;
	};
}