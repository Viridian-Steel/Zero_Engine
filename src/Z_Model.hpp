#pragma once
#include "Z_Buffer.hpp"
#include "Z_device.hpp"

//std
#include <vector>
#include <memory>

//glm lib
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>



namespace ze {
	class ZModel {
	public:

		struct Vertex {
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const {
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Builder {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filePath);
		};

		ZModel(ZDevice& device, const ZModel::Builder& builder);
		~ZModel();

		ZModel(const ZModel&) = delete;
		ZModel& operator=(const ZModel&) = delete;

		static std::unique_ptr<ZModel> creatModelFromFile(ZDevice& device, const std::string& filePath);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);



	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);

		ZDevice&		zDevice;

		std::unique_ptr<ZBuffer> vertexBuffer;
		uint32_t		vertexCount;

		std::unique_ptr<ZBuffer> indexBuffer;
		uint32_t		indexCount;

		bool			hasIndexBuffer = false;

	};
}