#include "Z_Renderer.hpp"

#include <cassert>
#include <stdexcept>
#include <array>


namespace ze {
	ZRenderer::ZRenderer(ZWindow& zWindow, ZDevice& zDevice) : zWindow{ zWindow }, zDevice{ zDevice }
	{
		recreateSwapChain();
		createCommandBuffers();
	}

	ZRenderer::~ZRenderer() { freeCommandBuffers(); }

	VkCommandBuffer ZRenderer::beginFrame()
	{
		assert(!isFrameStarted && "Can't call Begin Frame while frame already in progress!");

		auto result = zSwapChain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return nullptr;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to aqquire next image");
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Command buffer failed to start recording");
		}

		return commandBuffer;
	}

	void ZRenderer::endFrame()
	{
		assert(isFrameStarted && "Can't end frame while a frame is not in progress");
		auto commandBuffer = getCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer.");
		}

		auto result = zSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || zWindow.wasWindowResized())
		{
			zWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swapchain image");
		}

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % ZSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void ZRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't start the renderpass while a frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};

		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = zSwapChain->getRenderPass();
		renderPassInfo.framebuffer = zSwapChain->getFrameBuffer(currentImageIndex);
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = zSwapChain->getSwapChainExtent();


		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();


		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = static_cast<uint32_t>(zSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<uint32_t>(zSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;
		VkRect2D scissor{ {0,0}, zSwapChain->getSwapChainExtent() };

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void ZRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't stop the render pass while a frame is not in progress");
		assert(commandBuffer == getCurrentCommandBuffer() && "can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}

	void ZRenderer::recreateSwapChain()
	{
		auto extent = zWindow.getExtent();

		while (extent.width == 0 || extent.height == 0) {
			extent = zWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(zDevice.device());

		if (zSwapChain == nullptr) {
			zSwapChain = std::make_unique<ZSwapChain>(zDevice, extent);
		}
		else {
			std::shared_ptr<ZSwapChain> oldSwapChain = std::move(zSwapChain);
			zSwapChain = std::make_unique<ZSwapChain>(zDevice, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*zSwapChain.get())) { throw std::runtime_error("SwapChain image(or dpeth) format has changed!"); }
		}
		//we'll get back to here
	}

	void ZRenderer::createCommandBuffers()
	{
		commandBuffers.resize(zSwapChain->MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};

		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = zDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());


		if (vkAllocateCommandBuffers(zDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers.");
		}

	}

	void ZRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			zDevice.device(),
			zDevice.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();

	}

}

