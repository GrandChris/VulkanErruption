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