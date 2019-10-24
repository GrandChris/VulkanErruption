///////////////////////////////////////////////////////////////////////////////
// File: VulkanDynamicPointRenderObject.cpp
// Date: 24.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: An object drawn as constantly changing points
///////////////////////////////////////////////////////////////////////////////

#include "VulkanDynamicPointRenderObject.h"



DynamicPointRenderObject::uPtr DynamicPointRenderObject::createVulkan()
{
	return std::make_unique<VulkanDynamicPointRenderObject>();
}


void VulkanDynamicPointRenderObject::create(VulkanParticleRenderer& engine)
{
	createGraphicsPipeline(engine);
	createVertexBuffer(engine);
	createUniformBuffer(engine);
	createDescriptorSets(engine);
	createCommandBuffer(engine);
}

void VulkanDynamicPointRenderObject::draw(VulkanParticleRenderer& engine)
{
	drawFrame(engine);
}

void VulkanDynamicPointRenderObject::cleanup(VulkanParticleRenderer& engine)
{
	descriptorSets.clear();
	commandBuffers.clear();
	uniformBuffers.clear();
	uniformBuffersMemory.clear();
	graphicsPipeline.reset();
	pipelineLayout.reset();
}



vk::VertexInputBindingDescription VulkanDynamicPointRenderObject::getVertexBindingDescription()
{
	vk::VertexInputBindingDescription bindingDescription;
	bindingDescription.setBinding(0);
	bindingDescription.setStride(sizeof(Vertex));
	bindingDescription.setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescription;
}

std::vector<vk::VertexInputAttributeDescription> VulkanDynamicPointRenderObject::getVertexAttributeDescriptions()
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(2);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32G32Sfloat);
	attributeDescriptions[0].setOffset(offsetof(Vertex, pos));
	attributeDescriptions[1].setBinding(0);
	attributeDescriptions[1].setLocation(1);
	attributeDescriptions[1].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[1].setOffset(offsetof(Vertex, color));

	return attributeDescriptions;
}


void VulkanDynamicPointRenderObject::createGraphicsPipeline(VulkanParticleRenderer& engine)
{
	engine.createGraphicsPipeline(pipelineLayout, graphicsPipeline, vertShaderCode, fragShaderCode,
		getVertexBindingDescription(), getVertexAttributeDescriptions());
}

void VulkanDynamicPointRenderObject::createVertexBuffer(VulkanParticleRenderer& engine)
{
	vk::DeviceSize const bufferSize = sizeof(Vertex) * mVerticesSize;

	engine.createVertexBuffers(vertexBufferMemory, vertexBuffers, bufferSize);
}

void VulkanDynamicPointRenderObject::createUniformBuffer(VulkanParticleRenderer& engine)
{
	engine.createUniformBuffers(uniformBuffersMemory, uniformBuffers, sizeof(UniformBufferObject));
}

void VulkanDynamicPointRenderObject::createDescriptorSets(VulkanParticleRenderer& engine)
{
	engine.createDescriptorSets(descriptorSets, uniformBuffers, sizeof(UniformBufferObject));
}

void VulkanDynamicPointRenderObject::createCommandBuffer(VulkanParticleRenderer& engine)
{
	engine.createCommandBuffers(commandBuffers, 
		pipelineLayout, graphicsPipeline,
		vertexBuffers, 
		descriptorSets, 
		mVertices.size());
}



void VulkanDynamicPointRenderObject::drawFrame(VulkanParticleRenderer& engine)
{
	mVertices = mVerticesFunc();

	UniformBufferObject ubo;
	engine.drawFrame(commandBuffers, uniformBuffersMemory, ubo, vertexBufferMemory, mVertices);
}



