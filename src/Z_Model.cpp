#include "Z_Model.hpp"
#include <cassert>
#include <cstring>

namespace ze {
	ZModel::ZModel(ZDevice& device, const std::vector<Vertex>& vertices): zDevice{device}
	{
		createVertexBuffers(vertices);
	}
	ZModel::~ZModel()
	{
		vkDestroyBuffer(zDevice.device(), vertexBuffer, nullptr);
		vkFreeMemory(zDevice.device(), vertexBufferMemory, nullptr);
	}
	void ZModel::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}
	void ZModel::draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}
	void ZModel::createVertexBuffers(const std::vector<Vertex>& vertices)
	{
		vertexCount = static_cast<uint32_t>(vertices.size());
		assert(vertexCount >= 3 && "Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		zDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer,
			vertexBufferMemory
		);

		void* data;

		vkMapMemory(zDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);

		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));

		vkUnmapMemory(zDevice.device(), vertexBufferMemory);
	}
	std::vector<VkVertexInputBindingDescription> ZModel::Vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		bindingDescriptions[0].stride = sizeof(Vertex);

		return bindingDescriptions;

	}
	std::vector<VkVertexInputAttributeDescription> ZModel::Vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].offset = offsetof(Vertex, position);
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].offset = offsetof(Vertex, color);
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;

		return attributeDescriptions;
	}
}