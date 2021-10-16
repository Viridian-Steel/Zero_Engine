#pragma once

#include "Graphics/z_window.hpp"
#include "Graphics/Z_Pipeline.hpp"
#include "Graphics/Z_device.hpp"
#include "Graphics/Z_swap_chain.hpp"
#include "Graphics/Z_Model.hpp"

#include <memory>
#include <vector>

namespace ze {
	class FirstApp {

	public:
		static constexpr int width = 800;
		static constexpr int height = 600;

		void run();

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;
	private:
		void loadModels(); 
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		ZeWindow window{width, height, "Heey, Vulkan's up!"};
		ZDevice zDevice{window};

		std::unique_ptr<ZSwapChain> zSwapChain;

		std::unique_ptr<ZePipeline> zPipeline;

		VkPipelineLayout pipelineLayout;

		std::vector<VkCommandBuffer> commandBuffers;

		std::unique_ptr<ZModel> zModel;
	};
}