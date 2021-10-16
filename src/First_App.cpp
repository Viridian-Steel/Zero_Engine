#include "First_App.hpp"
#include <stdexcept>
#include <array>


namespace ze {
	FirstApp::FirstApp()
	{
		loadModels();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}
	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(zDevice.device(), pipelineLayout, nullptr);
	}
	void FirstApp::run()
	{
		while (!window.shouldClose()) {
			glfwPollEvents();
			drawFrame();
		}
	}

	void FirstApp::loadModels()
	{
		std::vector<ZModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.f, 0.f, 0.f}},
			{{0.5f, 0.5f}, {0.f, 1.f, 0.f}},
			{{-0.5f, 0.5f}, {0.f, 0.f, 1.f}}
		};
		zModel = std::make_unique<ZModel>(zDevice, vertices);
	}
	void FirstApp::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(zDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout.");
		}
	}
	void FirstApp::createPipeline()
	{

		assert(zSwapChain != nullptr && "Failed to create pipeline before swapchain");
		assert(pipelineLayout != nullptr && "cannot create Pipeline before Pipeline Layout");

		PipelineConfigInfo pipelineConfig{};
		ZePipeline::defaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.renderPass = zSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;

		zPipeline = std::make_unique<ZePipeline>(
			zDevice,
			"A:/Repos/Zero_Engine/src/Shaders/simple_shader.vert.spv", 
			"A:/Repos/Zero_Engine/src/Shaders/simple_shader.frag.spv",
			pipelineConfig
			);

	}
	void FirstApp::recreateSwapChain()
	{
		auto extent = window.getExtent();

		while (extent.width == 0 || extent.height == 0) {
			extent = window.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(zDevice.device());
		
		if (zSwapChain == nullptr) {
			zSwapChain = std::make_unique<ZSwapChain>(zDevice, extent);
		}
		else {
			zSwapChain = std::make_unique<ZSwapChain>(zDevice, extent, std::move(zSwapChain));
			if (zSwapChain->imageCount() != commandBuffers.size())
			{
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		createPipeline();

	}
	void FirstApp::createCommandBuffers()
	{
		commandBuffers.resize(zSwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};

		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = zDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());


		if (vkAllocateCommandBuffers(zDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers.");
		}

	}

	void FirstApp::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			zDevice.device(), 
			zDevice.getCommandPool(), 
			static_cast<uint32_t>(commandBuffers.size()), 
			commandBuffers.data());
		commandBuffers.clear();

	}

	void FirstApp::recordCommandBuffer(int imageIndex) {
		VkCommandBufferBeginInfo beginInfo{};

		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Command buffer failed to start recording");
		}

		VkRenderPassBeginInfo renderPassInfo{};

		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = zSwapChain->getRenderPass();
		renderPassInfo.framebuffer = zSwapChain->getFrameBuffer(imageIndex);
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = zSwapChain->getSwapChainExtent();


		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();


		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = static_cast<uint32_t>(zSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<uint32_t>(zSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;
		VkRect2D scissor{ {0,0}, zSwapChain->getSwapChainExtent() };

		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		zPipeline->bind(commandBuffers[imageIndex]);

		zModel->bind(commandBuffers[imageIndex]);
		zModel->draw(commandBuffers[imageIndex]);

		vkCmdEndRenderPass(commandBuffers[imageIndex]);

		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer.");
		}
	}
	void FirstApp::drawFrame()
	{
		uint32_t imageIndex;
		auto result = zSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to aqquire next image");
		}

		recordCommandBuffer(imageIndex);

		result = zSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
		{
			window.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swapchain image");
		}
	}
}

