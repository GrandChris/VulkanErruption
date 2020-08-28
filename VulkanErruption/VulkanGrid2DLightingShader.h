///////////////////////////////////////////////////////////////////////////////
// File: Grid2DLightingShader.h
// Date: 17.08.2020
// Version: 1
// Author: Christian Steinbrecher
// Description: Drawing a 2D Grid
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Grid2DLightingShader.h"

#include "shaders/generated/Grid2D_vert.h"
#include "shaders/generated/Grid2D_geom.h"
#include "shaders/generated/Grid2D_frag.h"

#include "shaders/generated/Grid2DLighting_geom.h"
#include "shaders/generated/Grid2DLighting_vert.h"

#include <vulkan/vulkan.hpp>

inline auto Grid2DLightingShader::getVertexAttributeDescriptions()
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions(1);

	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32Uint);
	attributeDescriptions[0].setOffset(offsetof(VertexBufferElement, lightColor));

	return attributeDescriptions;
}

inline auto Grid2DLightingShader::getSpecializationInfoVertexShader()
{
	return std::vector<vk::SpecializationMapEntry>();
}

inline auto Grid2DLightingShader::getSpecializationInfoGeometryShader()
{
	std::vector<vk::SpecializationMapEntry> specializationMap(1);

	specializationMap[0].setConstantID(0);
	specializationMap[0].setSize(sizeof(SpecializationInfoGeometryShader().useSpecularLighting));
	specializationMap[0].setOffset(offsetof(SpecializationInfoGeometryShader, useSpecularLighting));

	return specializationMap;
}

inline auto Grid2DLightingShader::getSpecializationInfoFragmentShader()
{
	return std::vector<vk::SpecializationMapEntry>();
}


inline std::vector<char> Grid2DLightingShader::getVertexShaderCode()
{
	return Grid2DLighting_vert_spv;
}

inline std::vector<char> Grid2DLightingShader::getGeometryShaderCode()
{
	return Grid2DLighting_geom_spv;
}

inline std::vector<char> Grid2DLightingShader::getFragmentShaderCode()
{
	return Grid2D_frag_spv;
}


