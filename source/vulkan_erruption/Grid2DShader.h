///////////////////////////////////////////////////////////////////////////////
// File: Grid2DShader.h
// Date: 17.08.2020
// Version: 1
// Author: Christian Steinbrecher
// Description: Drawing a 2D Grid
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>
#include <vector>


struct Grid2DShader
{
	struct VertexBufferElement
	{
		// for every point, it also needs the height of the 3 neighbouring points
		float height1; //  1------2
		float height2; //  |      |
		float height3; //  |      |
		float height4; //  3------4
	};

	struct StorageBufferElement
	{

	};

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

	// converts a single point 2D-Array into 4-point 2D-Array
	static std::vector<VertexBufferElement> convertToVertex(std::vector<float> const & vec, size_t const width);
};


// ########+++++++ Implementation +++++++#######


inline std::vector<Grid2DShader::VertexBufferElement> Grid2DShader::convertToVertex(std::vector<float> const & vec, size_t const sizeX)
{
	size_t const sizeY = vec.size() / (sizeX+1) -1;

	std::vector<Grid2DShader::VertexBufferElement> res((sizeX) * (sizeY));

	for (size_t y = 0; y < sizeY ; ++y) {
		for (size_t x = 0; x < sizeX; ++x) {
			size_t const i_src = y * sizeX + x;
			size_t const i_src2 = (y+1) * sizeX + x;
			size_t const i_res = y * (sizeX -1) + x;

			assert(i_src < vec.size());
			assert(i_src2 < vec.size());
			assert(i_res < res.size());
			
			res[i_res].height1 = vec[i_src     ];
			res[i_res].height2 = vec[i_src  + 1];
			res[i_res].height3 = vec[i_src2    ];
			res[i_res].height4 = vec[i_src2 + 1];
		}
	}

	return res;
}