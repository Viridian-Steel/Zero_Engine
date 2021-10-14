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
		void sierpinski(
			std::vector<ZModel::Vertex>& vertices,
			int depth,
			glm::vec2 left,
			glm::vec2 right,
			glm::vec2 top);
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		ZeWindow window{width, height, "Heey, Vulkan's up!"};
		ZDevice zDevice{window};

		ZSwapChain zSwapChain{zDevice, window.getExtent()};

		std::unique_ptr<ZePipeline> zPipeline;

		VkPipelineLayout pipelineLayout;

		std::vector<VkCommandBuffer> commandBuffers;

		std::unique_ptr<ZModel> zModel;
	};
}