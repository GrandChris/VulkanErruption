///////////////////////////////////////////////////////////////////////////////
// File: Grid2DLightingShader.h
// Date: 17.08.2020
// Version: 1
// Author: Christian Steinbrecher
// Description: Drawing a 2D Grid
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>
#include <vector>


struct Grid2DLightingShader
{
	struct VertexBufferElement
	{
		// for every point, is the colour for a quad
		//  1------2
		//  |      |
		//  |      |
		//  3------4
		unsigned int lightColor = glm::packUnorm4x8({ 0.82f, 0.94f, 1.0f, 1.0f });
	};


	// std340:		size		allignment
	// float		4			4	
	// vec2			2 * 4		8
	// vec3			4 * 4		16
	// vec4			4 * 4		16
	// float[3]		3 * 4		4
	struct StorageBufferElement
	{
		alignas(4) float height;
		alignas(4) float lightStrength;
	};


	// std140:		size		allignment
	// float		4           4
	// vec2			2 * 4       8
	// vec3			4 * 4       16
	// vec4			4 * 4       16
	// float[3]		3 * 4 * 4   16
	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		alignas(16) glm::vec3 ligthPos = glm::vec3(0.0f, 100.0f, 30.0f);
		alignas(16) glm::vec3 color = glm::vec3(1.0f, 0.0f, 1.0f);
		alignas(16) glm::uvec2 maxIndex;
	};


	struct SpecializationInfoVertexShader
	{

	};

	struct SpecializationInfoGeometryShader
	{
		uint32_t useSpecularLighting = 1;
	};

	struct SpecializationInfoFragmentShader
	{ 

	};

	static auto getVertexAttributeDescriptions();

	static auto getSpecializationInfoVertexShader();
	static auto getSpecializationInfoGeometryShader();
	static auto getSpecializationInfoFragmentShader();

	static std::vector<char> getVertexShaderCode();
	static std::vector<char> getGeometryShaderCode();
	static std::vector<char> getFragmentShaderCode();

};




