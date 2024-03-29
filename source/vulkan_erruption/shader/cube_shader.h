//
// @file:   cube_shader.h
// @author: GrandChris
// @date:   2021-09-07
// @brief:  Shader for drawing cubes
//

#pragma once 

#include "include_glm.h"

#include "vulkan_erruption/object/simple_object/hello_triangle_shader.h"
#include "vulkan_erruption/object/dynamic_point_object/dynamic_point_object_shader.h"

class CubeShader : public DynamicPointObjectShader
{
public:
	enum class LightingType 
	{
		None,
		Diffuse,
		Gouraud,
		Pong
	};

	struct VertexBufferElement {
		glm::vec3 pos;
		glm::vec3 color;
	};

	// https://www.oreilly.com/library/view/opengl-programming-guide/9780132748445/app09lev1sec2.html
	struct UnformBuffer {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
		alignas(16) glm::vec3 lightPosition;
		alignas(4)  float ambient;
    // float diffuse;
    // float specular;
	};

	CubeShader(LightingType light = LightingType::Pong);

	std::vector<char> getVertexShaderCode() const override;
	std::vector<char> getGeometryShaderCode() const override;
	std::vector<char> getFragmentShaderCode() const override;

	vk::PrimitiveTopology getInputTopology() const override;

    std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions() const override;
	vk::VertexInputBindingDescription getVertexBindingDescription() const override;

	std::vector<vk::DescriptorSetLayoutBinding> getUniformBindingDescription() const override;

	size_t getVertexElementSize() const override;
	size_t getUniformBufferSize() const override;

private:
	LightingType const mLight;
};

