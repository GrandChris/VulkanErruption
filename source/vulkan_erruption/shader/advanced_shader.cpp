//
// @file:   simple_shader.h
// @author: GrandChris
// @date:   2021-09-07
// @brief:  Shader for hello triangle
//

#include "advanced_shader.h"

#include "shaders/advanced_shader_none.vert.h"
#include "shaders/advanced_shader_none.frag.h"

#include "shaders/advanced_shader_diffuse.frag.h"
#include "shaders/advanced_shader_diffuse.vert.h"

AdvancedShader::AdvancedShader(LightingType light)
	: mLight(light)
{

}

std::vector<vk::VertexInputAttributeDescription> AdvancedShader::getVertexAttributeDescriptions() const
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

vk::VertexInputBindingDescription AdvancedShader::getVertexBindingDescription() const
{
	vk::VertexInputBindingDescription bindingDescription;
	bindingDescription.setBinding(0);
	bindingDescription.setStride(sizeof(VertexBufferElement));
	bindingDescription.setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescription;
}

std::vector<vk::DescriptorSetLayoutBinding> AdvancedShader::getUniformBindingDescription() const
{
	// Uniform Buffer Layout
	std::vector<vk::DescriptorSetLayoutBinding>  uboLayoutBinding;
	uboLayoutBinding.resize(1);
	uboLayoutBinding[0].setBinding(0);
	uboLayoutBinding[0].setDescriptorType(vk::DescriptorType::eUniformBuffer);
	uboLayoutBinding[0].setDescriptorCount(1); // if it is an array of objects
	uboLayoutBinding[0].setStageFlags(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);
	uboLayoutBinding[0].setPImmutableSamplers(nullptr); // Optional

	return uboLayoutBinding;
}

std::vector<char> AdvancedShader::getVertexShaderCode() const
{	
	if(mLight == LightingType::Diffuse) {
		return advanced_shader_diffuse_vert_spv;
	}
	else {
		return advanced_shader_none_vert_spv;
	}
}

std::vector<char> AdvancedShader::getFragmentShaderCode() const
{
	if(mLight == LightingType::Diffuse) {
		return advanced_shader_diffuse_frag_spv;
	}
	else {
		return advanced_shader_none_frag_spv;
	}
}


size_t AdvancedShader::getUniformBufferSize() const
{
	return sizeof(UnformBuffer);
}