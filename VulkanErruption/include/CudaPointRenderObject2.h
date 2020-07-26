

// Ugly not allowed to be included withoud concrete CudaExternalVertexBuffer<Vertex> declaration fix


#pragma once

#include "CudaPointRenderObject.h"


// #######+++++++ Implementation +++++++#######

template<typename TFunc>
inline void CudaPointRenderObject::setVertices(CudaExternalVertexBuffer<Vertex>& dp_ExternalVertexBuffer,
	size_t const VertexBufferSize, TFunc& funcObj)
{
	mExternalVertexBufferSize = VertexBufferSize;

	using std::placeholders::_1;
	using std::placeholders::_2;

	mInitExternalVertexBufferFunction =
		std::bind(&CudaExternalVertexBuffer<Vertex>::init, &dp_ExternalVertexBuffer, _1, _2);

	mVerticesFunc = funcObj;
}