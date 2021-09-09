//
// @file:   simple_shader.h
// @author: GrandChris
// @date:   2021-09-07
// @brief:  Shader for hello triangle
//

#pragma once 

#include "include_glm.h"

#include "vulkan_erruption/object/simple_object/hello_triangle_shader.h"
#include "vulkan_erruption/object/dynamic_point_object/dynamic_point_object_shader.h"

class AdvancedShader : public DynamicPointObjectShader
{
public:

	struct VertexBufferElement {
		glm::vec3 pos;
		glm::vec3 color;
	};

	struct UnformBuffer {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	std::vector<char> getVertexShaderCode() const override;
	std::vector<char> getFragmentShaderCode() const override;

    std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions() const override;
	vk::VertexInputBindingDescription getVertexBindingDescription() const override;

	std::vector<vk::DescriptorSetLayoutBinding> getUniformBindingDescription() const override;

	size_t getUniformBufferSize() const override;
};

