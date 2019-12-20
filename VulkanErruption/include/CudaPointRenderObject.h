///////////////////////////////////////////////////////////////////////////////
// File: CudaPointRenderObject.h
// Date: 20.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Position of poitns are set through a cuda kernel
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RenderObject.h"

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <functional>


// forward declaration
template<typename TVertex>
class CudaExternalVertexBuffer;


class CudaPointRenderObject : public RenderObject
{
public:
	using uPtr = std::unique_ptr<CudaPointRenderObject>;

	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;
	};

	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	template<typename TFunc>
	void setVertices(CudaExternalVertexBuffer<Vertex> & dp_ExternalVertexBuffer, size_t const VertexBufferSize,
		TFunc& funcObj);
	
	void setPosition(glm::vec3 const& pos);


	static uPtr createVulkan();

protected:
	// bool stands for if it is called on init
	std::function<void(bool)> mVerticesFunc;

	size_t mExternalVertexBufferSize = 0;

	std::function<void(void*, size_t)> mInitExternalVertexBufferFunction;


	glm::vec3 mPos;
};


// #######+++++++ Implementation +++++++#######

template<typename TFunc>
inline void CudaPointRenderObject::setVertices(CudaExternalVertexBuffer<Vertex> & dp_ExternalVertexBuffer,
	size_t const VertexBufferSize, TFunc& funcObj)
{
	mExternalVertexBufferSize = VertexBufferSize;

	using std::placeholders::_1;
	using std::placeholders::_2;

	mInitExternalVertexBufferFunction = 
		std::bind(&CudaExternalVertexBuffer<Vertex>::init, &dp_ExternalVertexBuffer, _1, _2);

	mVerticesFunc = funcObj;
}

inline void CudaPointRenderObject::setPosition(glm::vec3 const& pos)
{
	mPos = pos;
}


