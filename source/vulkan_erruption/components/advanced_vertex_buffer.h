//
// @file:   default_vertex_buffer.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  A default unifor buffer descriptor
//

#pragma once

#include "vulkan_erruption/shader_object/render_engine_interface.h"

#include <vulkan/vulkan.hpp>
#include "event/delegate.h"

class AdvancedVertexBuffer
{
public:
	void createVertexBuffer(RenderEngineInterface& engine, size_t vertexBufferSize);

    std::vector<vk::UniqueBuffer> const & getBuffers();

    void update(RenderEngineInterface& engine, size_t const imageIndex, Delegate<void(void * const, size_t)> & updateVertexBuffer);

    void clear();
private:
	// Vertex Buffer
	std::vector<vk::UniqueDeviceMemory> vertexBufferMemory;
	std::vector<vk::UniqueBuffer> vertexBuffers;

    size_t mVertexBufferSize = 0;

};


inline void AdvancedVertexBuffer::createVertexBuffer(RenderEngineInterface& engine, size_t vertexBufferSize) 
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

inline std::vector<vk::UniqueBuffer> const & AdvancedVertexBuffer::getBuffers()
{
    return vertexBuffers;
}

inline void AdvancedVertexBuffer::update(RenderEngineInterface& engine, size_t const imageIndex, Delegate<void(void * const, size_t)> & updateVertexBuffer)
{
    assert(!vertexBufferMemory.empty());
    assert(!vertexBuffers.empty());
    assert(updateVertexBuffer);

    auto& vertexMem = vertexBufferMemory[imageIndex].get();

    void * memMappedData = engine.getDevice().mapMemory(vertexMem, 0, mVertexBufferSize);

    updateVertexBuffer(memMappedData, mVertexBufferSize);

    engine.getDevice().unmapMemory(vertexMem);
}

inline void AdvancedVertexBuffer::clear() 
{
	vertexBuffers.clear();
	vertexBufferMemory.clear();
}