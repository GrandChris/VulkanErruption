//
// @file:   hello_triangle_shader.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  Shader for hello triangle
//

#pragma once 

#include <vulkan/vulkan.hpp>
#include <vector>

class HelloTriangleShader
{
public:
    virtual std::vector<char> getVertexShaderCode() const = 0;
	virtual std::vector<char> getFragmentShaderCode() const = 0;

    virtual std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions() const = 0;
    virtual vk::VertexInputBindingDescription getVertexBindingDescription() const = 0;

    virtual size_t getVertexBufferElementSize() const = 0;
};