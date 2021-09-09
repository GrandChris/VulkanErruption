//
// @file:   simple_descriptor_set_layout.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  A default unifor buffer descriptor
//

#pragma once

#include "vulkan_erruption/shader_object/render_engine_interface.h"

#include <vulkan/vulkan.hpp>

class SimpleDescriptorSetLayout
{
public:
	void createDescriptorSetLayout(RenderEngineInterface& engine, std::vector<vk::DescriptorSetLayoutBinding> const & description);

    vk::DescriptorSetLayout const & getDescriptorSetLayout();

    void clear();
private:
	// Uniform Buffer description
	vk::UniqueDescriptorSetLayout descriptorSetLayout;

};


inline void SimpleDescriptorSetLayout::createDescriptorSetLayout(RenderEngineInterface& engine, std::vector<vk::DescriptorSetLayoutBinding> const & description) 
{
	vk::DescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.setBindingCount(static_cast<uint32_t>(description.size()));
	layoutInfo.setPBindings(description.data());

	descriptorSetLayout = engine.getDevice().createDescriptorSetLayoutUnique(layoutInfo);	
}

inline vk::DescriptorSetLayout  const & SimpleDescriptorSetLayout::getDescriptorSetLayout()
{
    return descriptorSetLayout.get();
}

inline void SimpleDescriptorSetLayout::clear() 
{
	descriptorSetLayout.reset();
}