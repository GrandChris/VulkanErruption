//
// @file:   simple_uniform_buffer.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  A default unifor buffer descriptor
//

#pragma once

#include "vulkan_erruption/shader_object/render_engine_interface.h"

#include <vulkan/vulkan.hpp>
#include "event/delegate.h"

class SimpleUniformBuffer
{
public:
	void createUniformBuffer(RenderEngineInterface& engine, size_t uniformBufferSize);

    std::vector<vk::UniqueBuffer> const & getBuffers();

    void update(RenderEngineInterface& engine, size_t const imageIndex, Delegate<void(void * const, size_t)> & updateVertexBuffer);

    void clear();
private:
	// Uniform Buffers
	std::vector<vk::UniqueDeviceMemory> uniformBuffersMemory;
	std::vector<vk::UniqueBuffer> uniformBuffers;

    size_t mUniformBufferSize = 0;

};


inline void SimpleUniformBuffer::createUniformBuffer(RenderEngineInterface& engine, size_t uniformBufferSize) 
{
    mUniformBufferSize = uniformBufferSize;
	vk::DeviceSize const bufferSize = uniformBufferSize;

	uniformBuffers.resize(engine.getSwapChainSize());
	uniformBuffersMemory.resize(engine.getSwapChainSize());

	for (size_t i = 0; i < engine.getSwapChainSize(); ++i)
	{
		engine.createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			uniformBuffers[i], uniformBuffersMemory[i]
		);
	}
}

inline std::vector<vk::UniqueBuffer> const & SimpleUniformBuffer::getBuffers()
{
    return uniformBuffers;
}

inline void SimpleUniformBuffer::update(RenderEngineInterface& engine, size_t const imageIndex, Delegate<void(void * const, size_t)> & updateUniformBuffer)
{
    assert(!uniformBuffersMemory.empty());
    assert(!uniformBuffers.empty());
    assert(updateUniformBuffer);

    auto& vertexMem = uniformBuffersMemory[imageIndex].get();

    void * memMappedData = engine.getDevice().mapMemory(vertexMem, 0, mUniformBufferSize);

    updateUniformBuffer(memMappedData, mUniformBufferSize);

    engine.getDevice().unmapMemory(vertexMem);
}

inline void SimpleUniformBuffer::clear() 
{
	uniformBuffers.clear();
	uniformBuffersMemory.clear();
}