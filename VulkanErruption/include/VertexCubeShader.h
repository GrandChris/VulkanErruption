///////////////////////////////////////////////////////////////////////////////
// File: VertexCubeShader.h
// Date: 28.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Shader drawing points as cubes
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>
#include <vector>

enum class VertexCubeShaderType
{
	Pong,
	NoLighting
};

template<VertexCubeShaderType shaderType = VertexCubeShaderType::Pong>
struct VertexCubeShader
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
	};

	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	static auto getVertexAttributeDescriptions();
	static std::vector<char> getVertexShaderCode();
	static std::vector<char> getGeometryShaderCode();
	static std::vector<char> getFragmentShaderCode();
};