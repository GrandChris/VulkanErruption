///////////////////////////////////////////////////////////////////////////////
// File: DynamicPointRenderObject.h
// Date: 24.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Draws constantly changing particles
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RenderObject.h"

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <functional>


class DynamicPointRenderObject : public RenderObject
{
public:
	using uPtr = std::unique_ptr<DynamicPointRenderObject>;

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		float pointSize = 1.0f;
	};

	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	template<typename TFunc>
	void setVertices(TFunc & funcObj, size_t const arraySize);
	void setPosition(glm::vec3 const& pos);

	void setUseCubes(bool const val = false);


	static uPtr createVulkan();

protected:
	std::function<std::vector<Vertex>(void)> mVerticesFunc;

	std::vector<Vertex> mVertices;
	size_t mVerticesSize = 0;
	glm::vec3 mPos;

	bool mUseCubes = false;
};

// #######+++++++ Implementation +++++++#######

template<typename TFunc>
inline void DynamicPointRenderObject::setVertices(TFunc & funcObj, size_t const arraySize)
{
	mVerticesSize = arraySize;
	mVerticesFunc = funcObj;
	mVertices = funcObj();

	assert(mVerticesSize == mVertices.size());
}


inline void DynamicPointRenderObject::setPosition(glm::vec3 const& pos)
{
	mPos = pos;
}

inline void DynamicPointRenderObject::setUseCubes(bool const val)
{ 
	mUseCubes = val;
}
