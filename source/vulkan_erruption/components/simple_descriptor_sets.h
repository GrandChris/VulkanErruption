//
// @file:   simple_descriptor_sets.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  A default unifor buffer descriptor
//

#pragma once

#include "vulkan_erruption/shader_object/render_engine_interface.h"

#include <vulkan/vulkan.hpp>

class SimpleDescriptorSets
{
public:
	void createDescriptorSets(RenderEngineInterface& engine, 
        vk::DescriptorPool const & descriptorPool, vk::DescriptorSetLayout const& descriptorSetLayout, 
        std::vector<vk::UniqueBuffer> const& uniformBuffers, size_t const UniformBufferObjectSize);

    std::vector<vk::DescriptorSet>  const & getDescriptorSets();

    void clear();
private:
	// Descriptor sets
	std::vector<vk::DescriptorSet> descriptorSets;
};


inline void SimpleDescriptorSets::createDescriptorSets(RenderEngineInterface& engine, 
        vk::DescriptorPool const & descriptorPool, vk::DescriptorSetLayout const & descriptorSetLayout, 
        std::vector<vk::UniqueBuffer> const& uniformBuffers, size_t const UniformBufferObjectSize) 
{
    assert(!uniformBuffers.empty());

	std::vector<vk::DescriptorSetLayout> const layouts(engine.getSwapChainSize(), descriptorSetLayout);

	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.setDescriptorPool(descriptorPool);
	allocInfo.setDescriptorSetCount(static_cast<uint32_t>(engine.getSwapChainSize()));
	allocInfo.setPSetLayouts(layouts.data());

	descriptorSets = engine.getDevice().allocateDescriptorSets(allocInfo);

	for (size_t i = 0; i < engine.getSwapChainSize(); i++)
	{
		// Uniform Buffer
		vk::DescriptorBufferInfo uniformBufferInfo;
		uniformBufferInfo.setBuffer(uniformBuffers[i].get());
		uniformBufferInfo.setOffset(0);
		uniformBufferInfo.setRange(UniformBufferObjectSize);

		// // Storage Buffer
		// vk::DescriptorBufferInfo storageBufferInfo = {};
		// if (StorageBufferObjectSize > 0)
		// {
		// 	storageBufferInfo.setBuffer(storageBuffers[i].get());
		// 	storageBufferInfo.setOffset(0);
		// 	storageBufferInfo.setRange(StorageBufferObjectSize);
		// }
		
		vk::WriteDescriptorSet descriptorWritesUbo = {};
		descriptorWritesUbo.setDstSet(descriptorSets[i]);
		descriptorWritesUbo.setDstBinding(0);
		descriptorWritesUbo.setDstArrayElement(0);
		descriptorWritesUbo.setDescriptorType(vk::DescriptorType::eUniformBuffer);
		descriptorWritesUbo.setDescriptorCount(1);
		descriptorWritesUbo.setPBufferInfo(&uniformBufferInfo);
		descriptorWritesUbo.setPImageInfo(nullptr); // Optional
		descriptorWritesUbo.setPTexelBufferView(nullptr); // Optional

		// vk::WriteDescriptorSet descriptorWritesSbo = {};
		// descriptorWritesSbo.setDstSet(descriptorSets[i]);
		// descriptorWritesSbo.setDstBinding(1);
		// descriptorWritesSbo.setDstArrayElement(0);
		// descriptorWritesSbo.setDescriptorType(vk::DescriptorType::eStorageBuffer);
		// descriptorWritesSbo.setDescriptorCount(1);
		// descriptorWritesSbo.setPBufferInfo(&storageBufferInfo);
		// descriptorWritesSbo.setPImageInfo(nullptr); // Optional
		// descriptorWritesSbo.setPTexelBufferView(nullptr); // Optional

		// if (StorageBufferObjectSize > 0)
		// {	
		// 	std::array<vk::WriteDescriptorSet, 2> descriptorWrites = { descriptorWritesUbo , descriptorWritesSbo };
		// 	device->updateDescriptorSets(descriptorWrites, nullptr);
		// }
		// else
		// {	// Storage Buffer is not used
			std::array<vk::WriteDescriptorSet, 1> descriptorWrites = { descriptorWritesUbo };
			 engine.getDevice().updateDescriptorSets(descriptorWrites, nullptr);
		// }
	}
}

inline std::vector<vk::DescriptorSet> const & SimpleDescriptorSets::getDescriptorSets()
{
    return descriptorSets;
}

inline void SimpleDescriptorSets::clear() 
{
	descriptorSets.clear();
}