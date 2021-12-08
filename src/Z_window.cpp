#include "Z_window.hpp"
#include <stdexcept>


namespace ze {

	ZWindow::ZWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
		initWindow();
	}

	ZWindow::~ZWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void ZWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
	}

	void ZWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height)
	{
		auto zWindow = reinterpret_cast<ZWindow*>(glfwGetWindowUserPointer(window));
		zWindow->frameBufferResized = true;
		zWindow->width = width;
		zWindow->height = height;

	}

	void ZWindow::initWindow() {
		//GLFW initialization
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);	//Tell GLFW to not use OpenGL
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}
}