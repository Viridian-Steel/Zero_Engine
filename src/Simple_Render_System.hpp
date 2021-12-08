#pragma once

#include "Z_Pipeline.hpp"
#include "Z_device.hpp"
#include "Z_GameObject.hpp"

#include <memory>
#include <vector>

namespace ze {
	class SimpleRenderSystem {
	public:
		SimpleRenderSystem(ZDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
		
void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<ZGameObject>& gameObjects);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		ZDevice& zDevice;

		std::unique_ptr<ZePipeline> zPipeline;

		VkPipelineLayout pipelineLayout;
	};
}
