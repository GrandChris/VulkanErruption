///////////////////////////////////////////////////////////////////////////////
// File: VulkanTwoVertexCubeShader.h
// Date: 26.07.2020
// Version: 1
// Author: Christian Steinbrecher
// Description: Shader drawing points as cubes, Vulkan implementation
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "TwoVertexCubeShader.h"

#include "shaders/generated/two_vertex_cube_vert.h"
#include "shaders/generated/two_vertex_cube_specular_geom.h"
#include "shaders/generated/specular_frag.h"

#include <vulkan/vulkan.hpp>


inline auto TwoVertexCubeShader::getVertexAttributeDescriptions()
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(3);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[0].setOffset(offsetof(Vertex, pos1));

	attributeDescriptions[1].setBinding(0);
	attributeDescriptions[1].setLocation(1);
	attributeDescriptions[1].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[1].setOffset(offsetof(Vertex, pos2));

	attributeDescriptions[2].setBinding(0);
	attributeDescriptions[2].setLocation(2);
	attributeDescriptions[2].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[2].setOffset(offsetof(Vertex, color));

	return attributeDescriptions;
}

inline std::vector<char> TwoVertexCubeShader::getVertexShaderCode()
{
	return two_vertex_cube_vert_spv;
}

inline std::vector<char> TwoVertexCubeShader::getGeometryShaderCode()
{
	return two_vertex_cube_specular_geom_spv;
}

inline std::vector<char> TwoVertexCubeShader::getFragmentShaderCode()
{
	return specular_frag_spv;
}

