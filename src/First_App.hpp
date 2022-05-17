#pragma once

#include "Z_window.hpp"
#include "Z_device.hpp"
#include "Z_Renderer.hpp"
#include "Z_GameObject.hpp"
#include "Z_Descriptors.hpp"

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
		void loadGameObjects(); 

		ZWindow zWindow{width, height, "Heey, Vulkan's up!"};
		ZDevice zDevice{zWindow};
		ZRenderer zRenderer{zWindow , zDevice};

		//the order matters here	
		std::unique_ptr<ZDescriptorPool> globalPool{};
		ZGameObject::Map gameObjects;
	};
}