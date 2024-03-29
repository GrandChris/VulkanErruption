///////////////////////////////////////////////////////////////////////////////
// File: DynamicPointShaderObject.h
// Date: 24.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Draws constantly changing particles
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "event/event.h"
#include "vulkan_erruption/shader_object/shader_object.h"
#include "vulkan_erruption/object/dynamic_point_object/dynamic_point_data.h"
#include <glm/glm.hpp>
#include <span>

class DynamicPointObjectNoTemplate {
public:
    
};

template<typename TVertexData>
class DynamicPointObject : public ShaderObject 
{
public:
    Event<void(std::span<TVertexData> &)> updateVertexBuffer;

    DynamicPointObjectNoTemplate more;
};


// #include "VertexCubeShader.h"

// #include <glm/glm.hpp>

// #include <vector>
// #include <memory>
// #include <functional>


// template<typename TShader = VertexCubeShader<>>
// class DynamicPointRenderObject : public ShaderObject
// {
// public:
// 	using uPtr = typename std::unique_ptr<DynamicPointRenderObject>;

// 	using Shader = TShader;
// 	using VertexBufferElement = typename TShader::VertexBufferElement;
// 	using StorageBufferElement = typename TShader::StorageBufferElement;
// 	using UniformBufferObject = typename TShader::UniformBufferObject;
// 	using SpecializationInfoVertexShader = typename TShader::SpecializationInfoVertexShader;
// 	using SpecializationInfoGeometryShader = typename TShader::SpecializationInfoGeometryShader;
// 	using SpecializationInfoFragmentShader = typename TShader::SpecializationInfoFragmentShader;


// 	template<typename TFunc>
// 	void setVertexFunc(size_t const arraySize, TFunc& funcObj);

// 	template<typename TFunc>
// 	void setSboFunc(size_t const arraySize, TFunc& funcObj);

// 	template<typename TFunc>
// 	void setUbofunc(TFunc& funcObj);


// 	void setPosition(glm::vec3 const& pos);

// 	void setUbo(UniformBufferObject const& ubo);


// 	void setSpecializationInfoVertexShader(SpecializationInfoVertexShader const info);
// 	void setSpecializationInfoGeometryShader(SpecializationInfoGeometryShader const info);
// 	void setSpecializationInfoFragmentShader(SpecializationInfoFragmentShader const info);

// 	void setName(char const* name);


// 	static uPtr createVulkan();

// protected:

// 	// Vertex Buffer update function
// 	std::function<void(VertexBufferElement* begin, VertexBufferElement* end, bool fullRequired)> mVerticesFunc;

// 	// Storage Buffer update function
// 	std::function<void(StorageBufferElement* begin, StorageBufferElement* end, bool fullRequired)> mSBOFunc;

// 	// Uniform Buffer update function
// 	std::function<void(UniformBufferObject & ubo)> mUboFunc;


// 	size_t mVertexBufferSize = 0;
// 	size_t mStorageBufferSize = 0;
// 	glm::vec3 mPos;
// 	UniformBufferObject mUbo;
// 	SpecializationInfoVertexShader mSpecializationInfoVertexShader = {};
// 	SpecializationInfoGeometryShader mSpecializationInfoGeometryShader = {};
// 	SpecializationInfoFragmentShader mSpecializationInfoFragmentShader = {};

// 	char const* mName = "DPRO";
// };

// // #######+++++++ Implementation +++++++#######

// template<typename TShader>
// template<typename TFunc>
// inline void DynamicPointRenderObject<TShader>::setVertexFunc(size_t const arraySize, TFunc& funcObj)
// {
// 	mVertexBufferSize = arraySize;
// 	mVerticesFunc = funcObj;
// }

// template<typename TShader>
// template<typename TFunc>
// inline void DynamicPointRenderObject<TShader>::setSboFunc(size_t const arraySize, TFunc& funcObj)
// {
// 	mStorageBufferSize = arraySize;
// 	mSBOFunc = funcObj;
// }

// template<typename TShader>
// template<typename TFunc>
// inline void DynamicPointRenderObject<TShader>::setUbofunc(TFunc& funcObj)
// {
// 	mUboFunc = funcObj;
// }

// template<typename TShader>
// inline void DynamicPointRenderObject<TShader>::setPosition(glm::vec3 const& pos)
// {
// 	mPos = pos;
// }

// template<typename TShader>
// inline void DynamicPointRenderObject<TShader>::setUbo(UniformBufferObject const& ubo)
// {
// 	mUbo = ubo;
// }

// template<typename TShader>
// inline void DynamicPointRenderObject<TShader>::setSpecializationInfoVertexShader(SpecializationInfoVertexShader const info)
// {
// 	mSpecializationInfoVertexShader = std::move(info);
// }

// template<typename TShader>
// inline void DynamicPointRenderObject<TShader>::setSpecializationInfoGeometryShader(SpecializationInfoGeometryShader const info)
// {
// 	mSpecializationInfoGeometryShader = std::move(info);
// }

// template<typename TShader>
// inline void DynamicPointRenderObject<TShader>::setSpecializationInfoFragmentShader(SpecializationInfoFragmentShader const info)
// {
// 	mSpecializationInfoFragmentShader = std::move(info);

// }

// template<typename TShader>
// inline void DynamicPointRenderObject<TShader>::setName(char const* name)
// {
// 	mName = name;
// }

