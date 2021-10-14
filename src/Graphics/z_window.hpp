#pragma once
#define GLFW_INCLUDE_VULKAN

//includes
#include <GLFW/glfw3.h>
#include <string>

namespace ze {
	class ZeWindow {
	public:
		ZeWindow(int w, int h, std::string name);
		~ZeWindow();

		ZeWindow(const ZeWindow&) = delete;
		ZeWindow& operator=(const ZeWindow&) = delete;


		bool shouldClose() { return glfwWindowShouldClose(window); }

		VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:

		void initWindow();
		
		const int width;
		const int height;
		std::string windowName;

		GLFWwindow* window;
	};
}