#pragma once

#include "Z_Camera.hpp"
#include "Z_Pipeline.hpp"
#include "Z_device.hpp"
#include "Z_GameObject.hpp"
#include "Z_Frame_Info.hpp"

#include <memory>
#include <vector>

namespace ze {
	class SimpleRenderSystem {
	public:
		SimpleRenderSystem(ZDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
		
		void renderGameObjects(FrameInfo& frameInfo);

	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		ZDevice& zDevice;

		std::unique_ptr<ZePipeline> zPipeline;

		VkPipelineLayout pipelineLayout;
	};
}
