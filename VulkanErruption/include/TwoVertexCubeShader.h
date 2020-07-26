///////////////////////////////////////////////////////////////////////////////
// File: TwoVertexCubeShader.h
// Date: 28.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Shader drawing cubes from two edges
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>
#include <vector>

struct TwoVertexCubeShader
{
	struct Vertex
	{
		glm::vec3 pos1;
		glm::vec3 pos2;
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