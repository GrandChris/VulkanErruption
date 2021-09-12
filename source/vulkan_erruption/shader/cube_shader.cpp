//
// @file:   cube_shader.h
// @author: GrandChris
// @date:   2021-09-07
// @brief:  Shader for drawing cubes
//

#include "cube_shader.h"

#include "shaders/cube_shader.vert.h"
#include "shaders/cube_shader.geom.h"
#include "shaders/cube_shader.frag.h"

CubeShader::CubeShader(LightingType light)
	: mLight(light)
{

}

std::vector<vk::VertexInputAttributeDescription> CubeShader::getVertexAttributeDescriptions() const
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(2);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[0].setOffset(offsetof(VertexBufferElement, pos));

	attributeDescriptions[1].setBinding(0);
	attributeDescriptions[1].setLocation(1);
	attributeDescriptions[1].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[1].setOffset(offsetof(VertexBufferElement, color));

	return attributeDescriptions;
}

vk::VertexInputBindingDescription CubeShader::getVertexBindingDescription() const
{
	vk::VertexInputBindingDescription bindingDescription;
	bindingDescription.setBinding(0);
	bindingDescription.setStride(sizeof(VertexBufferElement));
	bindingDescription.setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescription;
}

std::vector<vk::DescriptorSetLayoutBinding> CubeShader::getUniformBindingDescription() const
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

std::vector<char> CubeShader::getVertexShaderCode() const
{	
	if(mLight == LightingType::Diffuse) {
		return  cube_shader_vert_spv;
	}
	else {
		return cube_shader_vert_spv;
	}
}

std::vector<char> CubeShader::getGeometryShaderCode() const
{	
	if(mLight == LightingType::Diffuse) {
		return  cube_shader_geom_spv;
	}
	else {
		return cube_shader_geom_spv;
	}
}

std::vector<char> CubeShader::getFragmentShaderCode() const
{
	if(mLight == LightingType::Diffuse) {
		return cube_shader_frag_spv;
	}
	else {
		return cube_shader_frag_spv;
	}
}

vk::PrimitiveTopology CubeShader::getInputTopology() const
{
	return vk::PrimitiveTopology::ePointList;
}

size_t CubeShader::getVertexElementSize() const
{
	return sizeof(VertexBufferElement);
}

size_t CubeShader::getUniformBufferSize() const
{
	return sizeof(UnformBuffer);
}