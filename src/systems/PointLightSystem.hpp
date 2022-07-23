#pragma once

#include "Z_Camera.hpp"
#include "Z_Pipeline.hpp"
#include "Z_device.hpp"
#include "Z_GameObject.hpp"
#include "Z_Frame_Info.hpp"

#include <memory>
#include <vector>

namespace ze {
	class PointLightSystem {
	public:
		PointLightSystem(ZDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;
		
		void render(FrameInfo& frameInfo);
		void update(FrameInfo& frameInfo, GlobalUBO& ubo);
	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		ZDevice& zDevice;

		std::unique_ptr<ZePipeline> zPipeline;

		VkPipelineLayout pipelineLayout;
	};
}
