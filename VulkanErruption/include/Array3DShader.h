///////////////////////////////////////////////////////////////////////////////
// File: Array3DShader.h
// Date: 29.02.2020
// Version: 1
// Author: Christian Steinbrecher
// Description: Drawing a 3dimensonal array as cubes
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>
#include <vector>

enum class eShader
{
	Pong,
	Gouraud,
	Diffuse,
	Points	
};

template<eShader shaderType = eShader::Pong>
struct Array3DShader
{
	struct Vertex
	{
		uint8_t color;
	};

	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		alignas(16) glm::uvec3 maxIndex;
	};

	static auto getVertexAttributeDescriptions();
	static std::vector<char> getVertexShaderCode();
	static std::vector<char> getGeometryShaderCode();
	static std::vector<char> getFragmentShaderCode();
};