//
// @file:   cube_array_shader.h
// @author: GrandChris
// @date:   2021-09-07
// @brief:  Shader for drawing cubes
//

#include "cube_array_shader.h"

#include "shaders/cube_array_shader.vert.h"
#include "shaders/cube_array_shader.geom.h"
#include "shaders/cube_array_shader.frag.h"

CubeArrayShader::CubeArrayShader(LightingType light)
	: mLight(light)
{

}


CubeArrayShader::CubeArrayShader(float cubeSize, uint32_t arrayLength, LightingType light)
: mLight(light),
  mVertexShaderSpecializationInfo{cubeSize, arrayLength},
  mGeometryShaderSpecializationInfo{cubeSize}
{

}

std::vector<vk::VertexInputAttributeDescription> CubeArrayShader::getVertexAttributeDescriptions() const
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(1);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32Uint);
	attributeDescriptions[0].setOffset(offsetof(VertexBufferElement, color));

	return attributeDescriptions;
}

vk::VertexInputBindingDescription CubeArrayShader::getVertexBindingDescription() const
{
	vk::VertexInputBindingDescription bindingDescription;
	bindingDescription.setBinding(0);
	bindingDescription.setStride(sizeof(VertexBufferElement));
	bindingDescription.setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescription;
}

std::vector<vk::DescriptorSetLayoutBinding> CubeArrayShader::getUniformBindingDescription() const
{
	// Uniform Buffer Layout
	std::vector<vk::DescriptorSetLayoutBinding>  uboLayoutBinding;
	uboLayoutBinding.resize(1);
	uboLayoutBinding[0].setBinding(0);
	uboLayoutBinding[0].setDescriptorType(vk::DescriptorType::eUniformBuffer);
	uboLayoutBinding[0].setDescriptorCount(1); // if it is an array of objects
	// uboLayoutBinding[0].setStageFlags(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);
	uboLayoutBinding[0].setStageFlags(vk::ShaderStageFlagBits::eGeometry);
	uboLayoutBinding[0].setPImmutableSamplers(nullptr); // Optional

	return uboLayoutBinding;
}

std::vector<vk::SpecializationMapEntry> CubeArrayShader::getVertexShaderSpecializationMap() const
{
	std::vector<vk::SpecializationMapEntry> specializationMapEntry;
	specializationMapEntry.resize(2);
	specializationMapEntry[0].setConstantID(0);
	specializationMapEntry[0].setSize(sizeof(SpecializationInfoVertexShader().cubeSize));
	specializationMapEntry[0].setOffset(offsetof(SpecializationInfoVertexShader, cubeSize));

	specializationMapEntry[1].setConstantID(1);
	specializationMapEntry[1].setSize(sizeof(SpecializationInfoVertexShader().arrayLength));
	specializationMapEntry[1].setOffset(offsetof(SpecializationInfoVertexShader, arrayLength));

	return specializationMapEntry;
}

std::vector<vk::SpecializationMapEntry> CubeArrayShader::getGeometryShaderSpecializationMap() const
{
	std::vector<vk::SpecializationMapEntry> specializationMapEntry;
	specializationMapEntry.resize(1);
	specializationMapEntry[0].setConstantID(0);
	specializationMapEntry[0].setSize(sizeof(SpecializationInfoGeometryShader().cubeSize));
	specializationMapEntry[0].setOffset(offsetof(SpecializationInfoGeometryShader, cubeSize));

	return specializationMapEntry;
}

std::vector<uint8_t> CubeArrayShader::getVertexShaderSpecializationData() const
{
	auto data = std::vector<uint8_t>(sizeof(SpecializationInfoVertexShader));
	memcpy(data.data(), &mVertexShaderSpecializationInfo, sizeof(SpecializationInfoVertexShader));

	return data;
}

std::vector<uint8_t> CubeArrayShader::getGeometryShaderSpecializationData() const
{
	auto data = std::vector<uint8_t>(sizeof(SpecializationInfoGeometryShader));
	memcpy(data.data(), &mGeometryShaderSpecializationInfo, sizeof(SpecializationInfoGeometryShader));

	return data;
}

std::vector<char> CubeArrayShader::getVertexShaderCode() const
{	
	if(mLight == LightingType::Diffuse) {
		return  cube_array_shader_vert_spv;
	}
	else {
		return cube_array_shader_vert_spv;
	}
}

std::vector<char> CubeArrayShader::getGeometryShaderCode() const
{	
	if(mLight == LightingType::Diffuse) {
		return  cube_array_shader_geom_spv;
	}
	else {
		return cube_array_shader_geom_spv;
	}
}

std::vector<char> CubeArrayShader::getFragmentShaderCode() const
{
	if(mLight == LightingType::Diffuse) {
		return cube_array_shader_frag_spv;
	}
	else {
		return cube_array_shader_frag_spv;
	}
}

vk::PrimitiveTopology CubeArrayShader::getInputTopology() const
{
	return vk::PrimitiveTopology::ePointList;
}

size_t CubeArrayShader::getVertexElementSize() const
{
	return sizeof(VertexBufferElement);
}

size_t CubeArrayShader::getUniformBufferSize() const
{
	return sizeof(UnformBuffer);
}