#pragma once

#include "Z_Camera.hpp"
#include "Z_GameObject.hpp"

#include <vulkan/vulkan.h>

namespace ze {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		ZCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		ZGameObject::Map& gameObjects;
	};
}