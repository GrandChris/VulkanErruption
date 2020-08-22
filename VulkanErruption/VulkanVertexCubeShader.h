///////////////////////////////////////////////////////////////////////////////
// File: VulkanVertexCubeShader.h
// Date: 28.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Shader drawing points as cubes, Vulkan implementation
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VertexCubeShader.h"

#include "shaders/generated/cube_vert.h"
#include "shaders/generated/cube_specular_geom.h"
#include "shaders/generated/specular_frag.h"

#include "shaders/generated/cube_geom.h"
#include "shaders/generated/shader_frag.h"

#include <vulkan/vulkan.hpp>

template<VertexCubeShaderType shaderType>
inline auto VertexCubeShader<shaderType>::getVertexAttributeDescriptions()
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

template<VertexCubeShaderType shaderType>
inline auto VertexCubeShader<shaderType>::getSpecializationInfoVertexShader()
{
	return std::vector<vk::SpecializationMapEntry>();
}

template<VertexCubeShaderType shaderType>
inline auto VertexCubeShader<shaderType>::getSpecializationInfoGeometryShader()
{
	std::vector<vk::SpecializationMapEntry> specializationMap(1);

	specializationMap[0].setConstantID(0);
	specializationMap[0].setSize(sizeof(SpecializationInfoGeometryShader().cubeSize));
	specializationMap[0].setOffset(offsetof(SpecializationInfoGeometryShader, cubeSize));

	return specializationMap;
}

template<VertexCubeShaderType shaderType>
inline auto VertexCubeShader<shaderType>::getSpecializationInfoFragmentShader()
{
	return std::vector<vk::SpecializationMapEntry>();
}


template<VertexCubeShaderType shaderType>
inline std::vector<char> VertexCubeShader<shaderType>::getVertexShaderCode()
{
	switch (shaderType) {
	default:
	case VertexCubeShaderType::Pong:
		return cube_vert_spv;
		break;
	case VertexCubeShaderType::NoLighting:
		return cube_vert_spv;
		break;
	};
}

template<VertexCubeShaderType shaderType>
inline std::vector<char> VertexCubeShader<shaderType>::getGeometryShaderCode()
{
	switch (shaderType) {
	default:
	case VertexCubeShaderType::Pong:
		return cube_specular_geom_spv;
		break;
	case VertexCubeShaderType::NoLighting:
		return cube_geom_spv;
		break;
	};
}

template<VertexCubeShaderType shaderType>
inline std::vector<char> VertexCubeShader<shaderType>::getFragmentShaderCode()
{
	switch (shaderType) {
	default:
	case VertexCubeShaderType::Pong:
		return specular_frag_spv;
		break;
	case VertexCubeShaderType::NoLighting:
		return shader_frag_spv;
		break;
	};
}


