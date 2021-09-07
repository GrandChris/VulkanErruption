//
// @file:   default_vertex_buffer.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  A default unifor buffer descriptor
//

#pragma once

#include "vulkan_erruption/shader_object/render_engine_interface.h"

#include <vulkan/vulkan.hpp>

class SimpleVertexBuffer
{
public:
	void createVertexBuffer(RenderEngineInterface& engine, size_t vertexBufferSize);

    std::vector<vk::UniqueBuffer> const & getBuffers();

    template<typename T>
    void update(RenderEngineInterface& engine, size_t const imageIndex, std::vector<T> const & data);

    void clear();
private:
	// Vertex Buffer
	std::vector<vk::UniqueDeviceMemory> vertexBufferMemory;
	std::vector<vk::UniqueBuffer> vertexBuffers;

    size_t mVertexBufferSize = 0;

};


inline void SimpleVertexBuffer::createVertexBuffer(RenderEngineInterface& engine, size_t vertexBufferSize) 
{
    mVertexBufferSize = vertexBufferSize;

	vk::DeviceSize const bufferSize = vertexBufferSize;
    size_t const swapChainSize = engine.getSwapChainSize();

	vertexBufferMemory.resize(swapChainSize);
	vertexBuffers.resize(swapChainSize);

	for (size_t i = 0; i < swapChainSize; ++i)
	{
		engine.createBuffer(bufferSize,  vk::BufferUsageFlagBits::eVertexBuffer,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eDeviceLocal,
			vertexBuffers[i], vertexBufferMemory[i]);
	}
}

inline std::vector<vk::UniqueBuffer> const & SimpleVertexBuffer::getBuffers()
{
    return vertexBuffers;
}

template<typename T>
inline void SimpleVertexBuffer::update(RenderEngineInterface& engine, size_t const imageIndex, std::vector<T> const & data)
{
    assert(!vertexBufferMemory.empty());
    assert(!vertexBuffers.empty());

    size_t const vertexBufferSize = sizeof(T) * data.size();
    assert(mVertexBufferSize == vertexBufferSize);

    auto& vertexMem = vertexBufferMemory[imageIndex].get();

    void * const memMappedData = engine.getDevice().mapMemory(vertexMem, 0, vertexBufferSize);

    memcpy(memMappedData, data.data(), vertexBufferSize);

    engine.getDevice().unmapMemory(vertexMem);
}

inline void SimpleVertexBuffer::clear() 
{
	vertexBuffers.clear();
	vertexBufferMemory.clear();
}