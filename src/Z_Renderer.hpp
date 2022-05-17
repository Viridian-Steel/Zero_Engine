#pragma once

#include "Z_device.hpp"
#include "Z_swap_chain.hpp"
#include "Z_window.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace ze {
	class ZRenderer {

	public:

		ZRenderer(ZWindow& zWindow, ZDevice& zDevice);
		~ZRenderer();

		ZRenderer(const ZRenderer&) = delete;
		ZRenderer& operator=(const ZRenderer&) = delete;

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

		VkRenderPass getSwapChainRenderPass() const { return zSwapChain->getRenderPass(); }
		float getAspectRatio() const { return zSwapChain->extentAspectRatio(); }
		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}
		bool isFrameinProgress() const { return isFrameStarted; }

		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		ZWindow& zWindow;
		ZDevice& zDevice;

		std::unique_ptr<ZSwapChain> zSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex{0};

		bool isFrameStarted = false;
	};
}