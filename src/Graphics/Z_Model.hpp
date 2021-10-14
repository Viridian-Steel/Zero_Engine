#pragma once
#include <vector>

#include "Z_device.hpp"


//glm lib
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>


namespace ze {
	class ZModel {
	public:

		struct Vertex {
			glm::vec2 position;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		ZModel(ZDevice& device, const std::vector<Vertex>& vertices);
		~ZModel();

		ZModel(const ZModel&) = delete;
		ZModel& operator=(const ZModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);



	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);

		ZDevice& zDevice;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount; 

	};
}