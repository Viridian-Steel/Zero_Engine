#include "First_App.hpp"
#include <stdexcept>
#include <array>


namespace ze {
	FirstApp::FirstApp()
	{
		loadModels();
		createPipelineLayout();
		createPipeline();
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

	void FirstApp::sierpinski(std::vector<ZModel::Vertex>& vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top) {
		if (depth <= 0) {
			vertices.push_back({ top });
			vertices.push_back({ right });
			vertices.push_back({ left });
		}
		else {
			auto leftTop = 0.5f * (left + top);
			auto rightTop = 0.5f * (right + top);
			auto leftRight = 0.5f * (left + right);
			sierpinski(vertices, depth - 1, left, leftRight, leftTop);
			sierpinski(vertices, depth - 1, leftRight, right, rightTop);
			sierpinski(vertices, depth - 1, leftTop, rightTop, top);
		}
	}

	void FirstApp::loadModels()
	{
		//std::vector<ZModel::Vertex> vertices{
		//	{{0.0f, -0.5f}},
		//	{{0.5f, 0.5f}},
		//	{{-0.5f, 0.5f}}
		//};
		std::vector<ZModel::Vertex> vertices{};
		sierpinski(vertices, 5, { -0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.0f, -0.5f });
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
		PipelineConfigInfo pipelineConfig{};
		ZePipeline::defaultPipelineConfigInfo(
			pipelineConfig,
			zSwapChain.width(),
			zSwapChain.height());

		pipelineConfig.renderPass = zSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;

		zPipeline = std::make_unique<ZePipeline>(
			zDevice,
			"A:/Repos/Zero_Engine/src/Shaders/simple_shader.vert.spv", 
			"A:/Repos/Zero_Engine/src/Shaders/simple_shader.frag.spv",
			pipelineConfig
			);

	}
	void FirstApp::createCommandBuffers()
	{
		commandBuffers.resize(zSwapChain.imageCount());

		VkCommandBufferAllocateInfo allocInfo{};

		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = zDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());


		if (vkAllocateCommandBuffers(zDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers.");
		}

		for (int i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};

			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("Command buffer failed to start recording");
			}

			VkRenderPassBeginInfo renderPassInfo{};

			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = zSwapChain.getRenderPass();
			renderPassInfo.framebuffer = zSwapChain.getFrameBuffer(i);
			renderPassInfo.renderArea.offset = {0,0};
			renderPassInfo.renderArea.extent = zSwapChain.getSwapChainExtent();


			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();


			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			zPipeline->bind(commandBuffers[i]);

			zModel->bind(commandBuffers[i]);
			zModel->draw(commandBuffers[i]);

			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer.");
			}
		}
	}
	void FirstApp::drawFrame()
	{
		uint32_t imageIndex;
		auto result = zSwapChain.acquireNextImage(&imageIndex);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to aqquire next image");
		}

		result = zSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swapchain image");
		}
	}
}

/*validation layer: Validation Error: [ VUID-VkPresentInfoKHR-pImageIndices-01296 ] Object 0: handle = 0x1e348952c78, type = VK_OBJECT_TYPE_QUEUE; | MessageID = 0xc7aabc16 | vkQueuePresentKHR(): pSwapchains[0] images passed to present must be in layout VK_IMAGE_LAYOUT_PRESENT_SRC_KHR or VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR but is in VK_IMAGE_LAYOUT_UNDEFINED. The Vulkan spec states: Each element of pImageIndices must be the index of a presentable image acquired from the swapchain specified by the corresponding element of the pSwapchains array, and the presented image subresource must be in the VK_IMAGE_LAYOUT_PRESENT_SRC_KHR layout at the time the operation is executed on a VkDevice (https://github.com/KhronosGroup/Vulkan-Docs/search?q=)VUID-VkPresentInfoKHR-pImageIndices-01296)*/