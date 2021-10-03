#include "First_App.hpp"

namespace ze {
	void FirstApp::run()
	{
		while (!window.shouldClose()) {
			glfwPollEvents();
		}
	}
}