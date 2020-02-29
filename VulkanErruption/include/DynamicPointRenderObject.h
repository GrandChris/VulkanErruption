///////////////////////////////////////////////////////////////////////////////
// File: DynamicPointRenderObject.h
// Date: 24.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Draws constantly changing particles
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RenderObject.h"

#include "VertexCubeShader.h"

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <functional>



template<typename T>
class TestInstantiation
{
public:
	void doStuff();
};


template<typename TShader = VertexCubeShader>
class DynamicPointRenderObject : public RenderObject
{
public:
	using uPtr = typename std::unique_ptr<DynamicPointRenderObject>;

	using Shader = TShader;
	using Vertex = typename TShader::Vertex;
	using UniformBufferObject = typename TShader::UniformBufferObject;


	template<typename TFunc>
	void setVertices(TFunc & funcObj, size_t const arraySize);
	void setPosition(glm::vec3 const& pos);
	void setUbo(UniformBufferObject const& ubo);



	static uPtr createVulkan();

protected:
	std::function<std::vector<Vertex>(void)> mVerticesFunc;

	std::vector<Vertex> mVertices;
	size_t mVerticesSize = 0;
	glm::vec3 mPos;
	UniformBufferObject mUbo;

};

// #######+++++++ Implementation +++++++#######

template<typename TShader>
template<typename TFunc>
inline void DynamicPointRenderObject<TShader>::setVertices(TFunc & funcObj, size_t const arraySize)
{
	mVerticesSize = arraySize;
	mVerticesFunc = funcObj;
	mVertices = funcObj();

	assert(mVerticesSize == mVertices.size());
}

template<typename TShader>
inline void DynamicPointRenderObject<TShader>::setPosition(glm::vec3 const& pos)
{
	mPos = pos;
}

template<typename TShader>
inline void DynamicPointRenderObject<TShader>::setUbo(UniformBufferObject const& ubo)
{
	mUbo = ubo;
}

