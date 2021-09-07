//
// @file:   simple_shader.h
// @author: GrandChris
// @date:   2021-09-07
// @brief:  Shader for hello triangle
//

#include "simple_shader.h"

#include "shaders/simple_shader.frag.h"
#include "shaders/simple_shader.vert.h"


std::vector<vk::VertexInputAttributeDescription> SimpleShader::getVertexAttributeDescriptions() const
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(1);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[0].setOffset(0);

	return attributeDescriptions;
}

vk::VertexInputBindingDescription SimpleShader::getVertexBindingDescription() const
{
	vk::VertexInputBindingDescription bindingDescription;
	bindingDescription.setBinding(0);
	bindingDescription.setStride(mVertexBufferElementSize);
	bindingDescription.setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescription;
}

std::vector<char> SimpleShader::getVertexShaderCode() const
{
	return simple_shader_vert_spv;
}

std::vector<char> SimpleShader::getFragmentShaderCode() const
{
	return simple_shader_frag_spv;
}

size_t SimpleShader::getVertexBufferElementSize() const 
{
	return mVertexBufferElementSize;
}