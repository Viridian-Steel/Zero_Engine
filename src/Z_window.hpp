#pragma once
#define GLFW_INCLUDE_VULKAN

//includes
#include <GLFW/glfw3.h>
#include <string>

namespace ze {
	class ZWindow {
	public:
		ZWindow(int w, int h, std::string name);
		~ZWindow();

		ZWindow(const ZWindow&) = delete;
		ZWindow& operator=(const ZWindow&) = delete;

		bool wasWindowResized() { return frameBufferResized; }

		void resetWindowResizedFlag() { frameBufferResized = false; }

		bool shouldClose() { return glfwWindowShouldClose(window); }

		VkExtent2D getExtent() {
			uint32_t extent_width = static_cast<uint32_t>(width);
			uint32_t extent_height = static_cast<uint32_t>(height);
			return {extent_width, extent_height};
		}

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		GLFWwindow* getGlfwWindow() const { return window; }

	private:
		static void frameBufferResizedCallback(GLFWwindow* window, int width, int height);

		void initWindow();
		
		int width;
		int height;

		bool frameBufferResized = false;
		std::string windowName;

		GLFWwindow* window;
	};
}