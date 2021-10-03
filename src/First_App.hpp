#pragma once

#include "Graphics/z_window.hpp"
#include "Graphics/Z_Pipeline.hpp"

namespace ze {
	class FirstApp {

	public:
		static constexpr int width = 800;
		static constexpr int height = 600;

		void run();

	private:
		ZeWindow window{width, height, "Heey, Vulkan's up!"};

		ZePipeline ZePipeline{ "A:/Repos/Zero_Engine/src/Shaders/simple_shader.vert.spv", "A:/Repos/Zero_Engine/src/Shaders/simple_shader.frag.spv" };
	};
}