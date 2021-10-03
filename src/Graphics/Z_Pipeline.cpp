#include"Z_Pipeline.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace ze {
	ZePipeline::ZePipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		createGraphicsPipeline(vertFilePath, fragFilePath);
	}

	std::vector<char> ZePipeline::readFile(const std::string& filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("fialed to open file: " + filePath);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());

		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	void ZePipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);

		std::cout << "Vertex Shader code Size: " << vertCode.size() << '\n';
		std::cout << "Fragment Shader code Size: "<<  fragCode.size() << '\n';
	}
}