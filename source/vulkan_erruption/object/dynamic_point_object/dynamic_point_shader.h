

#pragma once

#include "include_glm.h"
// #include <vulkan/vulkan.hpp>

class DynamicPointShader 
{
public:

	// using VertexBufferElement = PosColorVertex;

	// struct UniformBufferData
	// {
	// 	alignas(16) glm::mat4 model;
	// 	alignas(16) glm::mat4 view;
	// 	alignas(16) glm::mat4 proj;
	// };

	virtual std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions() const = 0;

	virtual std::vector<char> getVertexShaderCode() const = 0;
	virtual std::vector<char> getGeometryShaderCode() const = 0;
	virtual std::vector<char> getFragmentShaderCode() const = 0;


	struct SpecializationData {
		std::vector<vk::SpecializationMapEntry> entries;
		std::vector<uint8_t> data;
	};

	virtual SpecializationData getSpecializationInfoVertexShader() const = 0;
	virtual SpecializationData getSpecializationInfoGeometryShader() const = 0;
	virtual SpecializationData getSpecializationInfoFragmentShader() const = 0;

};