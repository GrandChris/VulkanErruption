///////////////////////////////////////////////////////////////////////////////
// File: Grid2DShader.h
// Date: 17.08.2020
// Version: 1
// Author: Christian Steinbrecher
// Description: Drawing a 2D Grid
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Grid2DShader.h"

#include "shaders/generated/Grid2D_vert.h"
#include "shaders/generated/Grid2D_geom.h"
#include "shaders/generated/Grid2D_frag.h"

#include <vulkan/vulkan.hpp>

inline auto Grid2DShader::getVertexAttributeDescriptions()
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions(4);

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

	return attributeDescriptions;
}

inline auto Grid2DShader::getSpecializationInfoVertexShader()
{
	return std::vector<vk::SpecializationMapEntry>();
}

inline auto Grid2DShader::getSpecializationInfoGeometryShader()
{
	std::vector<vk::SpecializationMapEntry> specializationMap(1);

	specializationMap[0].setConstantID(0);
	specializationMap[0].setSize(sizeof(SpecializationInfoGeometryShader().useSpecularLighting));
	specializationMap[0].setOffset(offsetof(SpecializationInfoGeometryShader, useSpecularLighting));

	return specializationMap;
}

inline auto Grid2DShader::getSpecializationInfoFragmentShader()
{
	return std::vector<vk::SpecializationMapEntry>();
}


inline std::vector<char> Grid2DShader::getVertexShaderCode()
{
	return Grid2D_vert_spv;
}

inline std::vector<char> Grid2DShader::getGeometryShaderCode()
{
	return Grid2D_geom_spv;
}

inline std::vector<char> Grid2DShader::getFragmentShaderCode()
{
	return Grid2D_frag_spv;
}


