//
// @file:   simple_descriptor_pool.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  A default unifor buffer descriptor
//

#pragma once

#include "vulkan_erruption/shader_object/render_engine_interface.h"

#include <vulkan/vulkan.hpp>
#include "event/delegate.h"

class SimpleDescriptorPool
{
public:
	void createDescriptorPool(RenderEngineInterface& engine);

    vk::DescriptorPool const & getDescriptorPool();

    void clear();
private:
	// Descriptor Pool
	vk::UniqueDescriptorPool descriptorPool;

};


inline void SimpleDescriptorPool::createDescriptorPool(RenderEngineInterface& engine) 
{
	std::array<vk::DescriptorPoolSize, 1> poolSize;
	poolSize[0].setType(vk::DescriptorType::eUniformBuffer);
	poolSize[0].setDescriptorCount(static_cast<uint32_t>(engine.getSwapChainSize()));
	// poolSize[1].setType(vk::DescriptorType::eStorageBuffer);
	// poolSize[1].setDescriptorCount(static_cast<uint32_t>(swapChainImages.size()));

	vk::DescriptorPoolCreateInfo poolInfo;
	poolInfo.setPoolSizeCount(static_cast<uint32_t>(poolSize.size()));
	poolInfo.setPPoolSizes(poolSize.data());
	poolInfo.setMaxSets(static_cast<uint32_t>(engine.getSwapChainSize()));

	descriptorPool = engine.getDevice().createDescriptorPoolUnique(poolInfo);
}

inline vk::DescriptorPool const &  SimpleDescriptorPool::getDescriptorPool()
{
    return descriptorPool.get();
}


inline void SimpleDescriptorPool::clear() 
{
    descriptorPool.reset();
}