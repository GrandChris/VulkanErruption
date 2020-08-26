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
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions(9);

	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32Sfloat);
	attributeDescriptions[0].setOffset(offsetof(Vertex, height1));

	attributeDescriptions[1].setBinding(0);
	attributeDescriptions[1].setLocation(1);
	attributeDescriptions[1].setFormat(vk::Format::eR32Sfloat);
	attributeDescriptions[1].setOffset(offsetof(Vertex, height2));

	attributeDescriptions[2].setBinding(0);
	attributeDescriptions[2].setLocation(2);
	attributeDescriptions[2].setFormat(vk::Format::eR32Sfloat);
	attributeDescriptions[2].setOffset(offsetof(Vertex, height3));

	attributeDescriptions[3].setBinding(0);
	attributeDescriptions[3].setLocation(3);
	attributeDescriptions[3].setFormat(vk::Format::eR32Sfloat);
	attributeDescriptions[3].setOffset(offsetof(Vertex, height4));

	attributeDescriptions[4].setBinding(0);
	attributeDescriptions[4].setLocation(4);
	attributeDescriptions[4].setFormat(vk::Format::eR32Sfloat);
	attributeDescriptions[4].setOffset(offsetof(Vertex, lightStrength1));

	attributeDescriptions[5].setBinding(0);
	attributeDescriptions[5].setLocation(5);
	attributeDescriptions[5].setFormat(vk::Format::eR32Sfloat);
	attributeDescriptions[5].setOffset(offsetof(Vertex, lightStrength2));

	attributeDescriptions[6].setBinding(0);
	attributeDescriptions[6].setLocation(6);
	attributeDescriptions[6].setFormat(vk::Format::eR32Sfloat);
	attributeDescriptions[6].setOffset(offsetof(Vertex, lightStrength3));

	attributeDescriptions[7].setBinding(0);
	attributeDescriptions[7].setLocation(7);
	attributeDescriptions[7].setFormat(vk::Format::eR32Sfloat);
	attributeDescriptions[7].setOffset(offsetof(Vertex, lightStrength4));

	attributeDescriptions[8].setBinding(0);
	attributeDescriptions[8].setLocation(8);
	//attributeDescriptions[8].setFormat(vk::Format::eR32Uint);
	attributeDescriptions[8].setFormat(vk::Format::eR32Uint);
	attributeDescriptions[8].setOffset(offsetof(Vertex, lightColor));

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


