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
	createDescriptorSetLayout(engine);
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
	attributeDescriptions.resize(3);
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[0].setOffset(offsetof(Vertex, pos));

	attributeDescriptions[1].setBinding(0);
	attributeDescriptions[1].setLocation(1);
	attributeDescriptions[1].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[1].setOffset(offsetof(Vertex, color));

	attributeDescriptions[2].setBinding(0);
	attributeDescriptions[2].setLocation(2);
	attributeDescriptions[2].setFormat(vk::Format::eR32Sfloat);
	attributeDescriptions[2].setOffset(offsetof(Vertex, pointSize));

	return attributeDescriptions;
}


void VulkanDynamicPointRenderObject::createDescriptorSetLayout(VulkanParticleRenderer& engine)
{
	if (mUseCubes == true)
	{
		engine.createDescriptorSetLayout(descriptorSetLayout, vk::ShaderStageFlagBits::eGeometry);
	}
	else
	{
		engine.createDescriptorSetLayout(descriptorSetLayout, vk::ShaderStageFlagBits::eVertex);
	}
}

void VulkanDynamicPointRenderObject::createGraphicsPipeline(VulkanParticleRenderer& engine)
{
	if (mUseCubes == true)
	{
		engine.createGraphicsPipeline(pipelineLayout, graphicsPipeline, vertGeomShaderCode, geomSpecularShaderCode, fragSpecularhaderCode,
			descriptorSetLayout, getVertexBindingDescription(), getVertexAttributeDescriptions());
	}
	else
	{
		engine.createGraphicsPipeline(pipelineLayout, graphicsPipeline, vertShaderCode, fragShaderCode,
			descriptorSetLayout, getVertexBindingDescription(), getVertexAttributeDescriptions());
	}
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
	engine.createDescriptorSets(descriptorSets, descriptorSetLayout, uniformBuffers, sizeof(UniformBufferObject));
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
	ubo.model = glm::translate(glm::mat4(1.0f), mPos);
	engine.drawFrame(commandBuffers, uniformBuffersMemory, ubo, vertexBufferMemory, mVertices);
}



