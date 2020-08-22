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


template<typename TShader = VertexCubeShader<>>
class DynamicPointRenderObject : public RenderObject
{
public:
	using uPtr = typename std::unique_ptr<DynamicPointRenderObject>;

	using Shader = TShader;
	using Vertex = typename TShader::Vertex;
	using UniformBufferObject = typename TShader::UniformBufferObject;
	using SpecializationInfoVertexShader = typename TShader::SpecializationInfoVertexShader;
	using SpecializationInfoGeometryShader = typename TShader::SpecializationInfoGeometryShader;
	using SpecializationInfoFragmentShader = typename TShader::SpecializationInfoFragmentShader;


	template<typename TFunc>
	void setVertices(TFunc & funcObj, size_t const arraySize);

	void setPosition(glm::vec3 const& pos);

	void setUbo(UniformBufferObject const& ubo);

	template<typename TFunc>
	void setUbofunc(TFunc& funcObj);

	void setSpecializationInfoVertexShader(SpecializationInfoVertexShader const info);
	void setSpecializationInfoGeometryShader(SpecializationInfoGeometryShader const info);
	void setSpecializationInfoFragmentShader(SpecializationInfoFragmentShader const info);


	static uPtr createVulkan();

protected:
	std::function<void(Vertex* begin, Vertex* end)> mVerticesFunc;

	std::function<void(UniformBufferObject & ubo)> mUboFunc;

	size_t mVerticesSize = 0;
	glm::vec3 mPos;
	UniformBufferObject mUbo;
	SpecializationInfoVertexShader mSpecializationInfoVertexShader = {};
	SpecializationInfoGeometryShader mSpecializationInfoGeometryShader = {};
	SpecializationInfoFragmentShader mSpecializationInfoFragmentShader = {};

};

// #######+++++++ Implementation +++++++#######

template<typename TShader>
template<typename TFunc>
inline void DynamicPointRenderObject<TShader>::setVertices(TFunc & funcObj, size_t const arraySize)
{
	mVerticesSize = arraySize;
	mVerticesFunc = funcObj;
}

template<typename TShader>
template<typename TFunc>
inline void DynamicPointRenderObject<TShader>::setUbofunc(TFunc& funcObj)
{
	mUboFunc = funcObj;
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

template<typename TShader>
inline void DynamicPointRenderObject<TShader>::setSpecializationInfoVertexShader(SpecializationInfoVertexShader const info)
{
	mSpecializationInfoVertexShader = std::move(info);
}

template<typename TShader>
inline void DynamicPointRenderObject<TShader>::setSpecializationInfoGeometryShader(SpecializationInfoGeometryShader const info)
{
	mSpecializationInfoGeometryShader = std::move(info);
}

template<typename TShader>
inline void DynamicPointRenderObject<TShader>::setSpecializationInfoFragmentShader(SpecializationInfoFragmentShader const info)
{
	mSpecializationInfoFragmentShader = std::move(info);

}

