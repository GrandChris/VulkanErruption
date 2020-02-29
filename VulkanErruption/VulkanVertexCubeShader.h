///////////////////////////////////////////////////////////////////////////////
// File: VulkanVertexCubeShader.h
// Date: 28.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Shader drawing points as cubes, Vulkan implementation
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VertexCubeShader.h"

#include "shaders/generated/cube_vert_spv.h"
#include "shaders/generated/cube_specular_geom_spv.h"
#include "shaders/generated/specular_frag_spv.h"

#include <vulkan/vulkan.hpp>


inline auto VertexCubeShader::getVertexAttributeDescriptions()
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(2);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[0].setOffset(offsetof(Vertex, pos));

	attributeDescriptions[1].setBinding(0);
	attributeDescriptions[1].setLocation(1);
	attributeDescriptions[1].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[1].setOffset(offsetof(Vertex, color));

	return attributeDescriptions;
}

inline std::vector<char> VertexCubeShader::getVertexShaderCode()
{
	return cube_vert_spv;
}

inline std::vector<char> VertexCubeShader::getGeometryShaderCode()
{
	return cube_specular_geom_spv;
}

inline std::vector<char> VertexCubeShader::getFragmentShaderCode()
{
	return specular_frag_spv;
}


