///////////////////////////////////////////////////////////////////////////////
// File: VulkanTwoPointMovingRenderObject.cpp
// Date: 19.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: An object drawn as static points
///////////////////////////////////////////////////////////////////////////////

#include "VulkanTwoPointMovingRenderObject.h"

//

TwoPointMovingRenderObject::uPtr TwoPointMovingRenderObject::createVulkan()
{
	return std::make_unique<VulkanTwoPointMovingRenderObject>();
}


void VulkanTwoPointMovingRenderObject::create(VulkanParticleRenderer& engine)
{
	createGraphicsPipeline(engine);
	createVertexBuffer(engine);
	createUniformBuffer(engine);
	createDescriptorSets(engine);
	createCommandBuffer(engine);
}

void VulkanTwoPointMovingRenderObject::draw(VulkanParticleRenderer& engine)
{
	drawFrame(engine);
}

void VulkanTwoPointMovingRenderObject::cleanup(VulkanParticleRenderer& engine)
{
	descriptorSets.clear();
	commandBuffers.clear();
	uniformBuffers.clear();
	uniformBuffersMemory.clear();
	graphicsPipeline.reset();
	pipelineLayout.reset();
}



vk::VertexInputBindingDescription VulkanTwoPointMovingRenderObject::getVertexBindingDescription()
{
	vk::VertexInputBindingDescription bindingDescription;
	bindingDescription.setBinding(0);
	bindingDescription.setStride(sizeof(Vertex));
	bindingDescription.setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescription;
}

std::vector<vk::VertexInputAttributeDescription> VulkanTwoPointMovingRenderObject::getVertexAttributeDescriptions()
{
	std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(3);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32G32Sfloat);
	attributeDescriptions[0].setOffset(offsetof(Vertex, posA));

	attributeDescriptions[1].setBinding(0);
	attributeDescriptions[1].setLocation(1);
	attributeDescriptions[1].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[1].setOffset(offsetof(Vertex, posB));

	attributeDescriptions[2].setBinding(0);
	attributeDescriptions[2].setLocation(2);
	attributeDescriptions[2].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[2].setOffset(offsetof(Vertex, color));

	return attributeDescriptions;
}


void VulkanTwoPointMovingRenderObject::createGraphicsPipeline(VulkanParticleRenderer& engine)
{
	engine.createGraphicsPipeline(pipelineLayout, graphicsPipeline, vertShaderCode, fragShaderCode,
		getVertexBindingDescription(), getVertexAttributeDescriptions());
}

void VulkanTwoPointMovingRenderObject::createVertexBuffer(VulkanParticleRenderer& engine)
{
	engine.createVertexBuffer(vertexBufferMemory, vertexBuffer, mVertices);
}

void VulkanTwoPointMovingRenderObject::createUniformBuffer(VulkanParticleRenderer& engine)
{
	engine.createUniformBuffers(uniformBuffersMemory, uniformBuffers, sizeof(UniformBufferObject));
}

void VulkanTwoPointMovingRenderObject::createDescriptorSets(VulkanParticleRenderer& engine)
{
	engine.createDescriptorSets(descriptorSets, uniformBuffers, sizeof(UniformBufferObject));
}

void VulkanTwoPointMovingRenderObject::createCommandBuffer(VulkanParticleRenderer& engine)
{
	engine.createCommandBuffers(commandBuffers, pipelineLayout, graphicsPipeline,
		vertexBuffer, descriptorSets, mVertices.size());
}



void VulkanTwoPointMovingRenderObject::drawFrame(VulkanParticleRenderer& engine)
{
	static auto const startTime = std::chrono::high_resolution_clock::now();
	auto const currentTime = std::chrono::high_resolution_clock::now();
	float const time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo;
	ubo.time = time;
	engine.drawFrame(commandBuffers, uniformBuffersMemory, ubo);
}

