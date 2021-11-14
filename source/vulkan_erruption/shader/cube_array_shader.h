//
// @file:   cube_shader.h
// @author: GrandChris
// @date:   2021-09-07
// @brief:  Shader for drawing an array of cubes
//

#pragma once 

#include "vulkan_erruption/object/dynamic_point_object/dynamic_point_object_shader.h"
#include "include_glm.h"

class CubeArrayShader : public DynamicPointObjectShader
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
		glm::uint color;
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

	struct SpecializationInfoVertexShader
	{
		float cubeSize = 1.0f;
		uint32_t arrayLength = 16.0f;
	};

	struct SpecializationInfoGeometryShader
	{
		float cubeSize = 1.0f;
	};

	CubeArrayShader(float cubeSize = 1.0f, uint32_t arrayLength = 16.0f, LightingType light = LightingType::Pong);

	void setSpecialization(float const cubeSize, uint32_t arrayLength) {
		mVertexShaderSpecializationInfo.cubeSize = cubeSize;
		mVertexShaderSpecializationInfo.arrayLength = arrayLength;
		mGeometryShaderSpecializationInfo.cubeSize = cubeSize;
	}

	std::vector<char> getVertexShaderCode() const override;
	std::vector<char> getGeometryShaderCode() const override;
	std::vector<char> getFragmentShaderCode() const override;

	vk::PrimitiveTopology getInputTopology() const override;

    std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions() const override;
	vk::VertexInputBindingDescription getVertexBindingDescription() const override;

	std::vector<vk::DescriptorSetLayoutBinding> getUniformBindingDescription() const override;

	size_t getVertexElementSize() const override;
	size_t getUniformBufferSize() const override;

    std::vector<vk::SpecializationMapEntry> getVertexShaderSpecializationMap() const override;
    std::vector<vk::SpecializationMapEntry> getGeometryShaderSpecializationMap() const override;

    std::vector<uint8_t> getVertexShaderSpecializationData() const override;
    std::vector<uint8_t> getGeometryShaderSpecializationData() const override;

private:
	LightingType const mLight;
	SpecializationInfoVertexShader mVertexShaderSpecializationInfo = {};
	SpecializationInfoVertexShader mGeometryShaderSpecializationInfo = {};
};

