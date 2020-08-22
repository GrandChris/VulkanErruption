///////////////////////////////////////////////////////////////////////////////
// File: VulkanArray3DShader
// Date: 29.02.2020
// Version: 1
// Author: Christian Steinbrecher
// Description: Drawing a 3dimensonal array as cubes, Vulkan implementation
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Array3DShader.h"

#include "shaders/generated/array3D_cube_vert.h"
#include "shaders/generated/array3D_cube_specular_geom.h"
#include "shaders/generated/specular_frag.h"

#include "shaders/generated/shader_frag.h"
#include "shaders/generated/array3D_cube_geom.h"


#include "shaders/generated/array3D_vert.h"
#include "shaders/generated/shader_vert.h"
#include "shaders/generated/shader_frag.h"



#include <vulkan/vulkan.hpp>

template<eShader shaderType>
inline auto Array3DShader<shaderType>::getVertexAttributeDescriptions()
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(1);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR8Uint);
	attributeDescriptions[0].setOffset(offsetof(Vertex, color));

	return attributeDescriptions;
}

template<eShader shaderType>
inline auto Array3DShader<shaderType>::getSpecializationInfoVertexShader()
{
	return std::vector<vk::SpecializationMapEntry>();
}

template<eShader shaderType>
inline auto Array3DShader<shaderType>::getSpecializationInfoGeometryShader()
{
	return std::vector<vk::SpecializationMapEntry>();
}

template<eShader shaderType>
inline auto Array3DShader<shaderType>::getSpecializationInfoFragmentShader()
{
	return std::vector<vk::SpecializationMapEntry>();
}

template<eShader shaderType>
inline std::vector<char> Array3DShader<shaderType>::getVertexShaderCode()
{
	switch (shaderType) {
	case eShader::Pong:
		return array3D_cube_vert_spv;
		break;
	case eShader::Gouraud:
		return array3D_cube_vert_spv;
		break;
	case eShader::Diffuse:
		return array3D_cube_vert_spv;
		break;
	case eShader::Points:
		return array3D_vert_spv;
		break;
	default: // use Pong
		return array3D_cube_vert_spv;
		break;
	};
}

template<eShader shaderType>
inline std::vector<char> Array3DShader<shaderType>::getGeometryShaderCode()
{
	switch (shaderType) {
	case eShader::Pong:
		return array3D_cube_specular_geom_spv;
		break;
	case eShader::Gouraud:
		return array3D_cube_specular_geom_spv;
		break;
	case eShader::Diffuse:
		return array3D_cube_geom_spv;
		break;
	case eShader::Points:
		return std::vector<char>();
		break;
	default: // use Pong
		return array3D_cube_specular_geom_spv;
		break;
	};
}

template<eShader shaderType>
inline std::vector<char> Array3DShader<shaderType>::getFragmentShaderCode()
{
	switch (shaderType) {
	case eShader::Pong:
		return specular_frag_spv;
		break;
	case eShader::Gouraud:
		return specular_frag_spv;
		break;
	case eShader::Diffuse:
		return shader_frag_spv;
		break;
	case eShader::Points:
		return shader_frag_spv;
		break;
	default:	// use Pong
		return specular_frag_spv;
		break;
	};
}


