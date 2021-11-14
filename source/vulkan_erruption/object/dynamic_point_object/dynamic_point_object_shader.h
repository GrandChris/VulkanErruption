//
// @file:   dynamic_point_object_shader.h
// @author: GrandChris
// @date:   2021-09-08
// @brief:  Shader for dynamic point object
//

#pragma once 

#include <vulkan/vulkan.hpp>
#include <vector>

class DynamicPointObjectShader
{
public:
    virtual std::vector<char> getVertexShaderCode() const = 0;
    virtual std::vector<char> getGeometryShaderCode() const = 0;
	virtual std::vector<char> getFragmentShaderCode() const = 0;

    virtual vk::PrimitiveTopology getInputTopology() const = 0;

    virtual std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions() const = 0;
    virtual vk::VertexInputBindingDescription getVertexBindingDescription() const = 0;
    virtual std::vector<vk::DescriptorSetLayoutBinding> getUniformBindingDescription() const = 0;

    virtual size_t getVertexElementSize() const = 0;
    virtual size_t getUniformBufferSize() const = 0;

    virtual std::vector<vk::SpecializationMapEntry> getVertexShaderSpecializationMap() const   {return {};}
    virtual std::vector<vk::SpecializationMapEntry> getGeometryShaderSpecializationMap() const {return {};}
    virtual std::vector<vk::SpecializationMapEntry> getFragmentShaderSpecializationMap() const {return {};}

    virtual std::vector<uint8_t> getVertexShaderSpecializationData() const    {return {};}
    virtual std::vector<uint8_t> getGeometryShaderSpecializationData() const  {return {};}
    virtual std::vector<uint8_t> getFragmentShaderSpecializationData() const  {return {};}
};