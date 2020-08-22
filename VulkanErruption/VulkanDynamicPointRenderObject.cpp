///////////////////////////////////////////////////////////////////////////////
// File: VulkanDynamicPointRenderObject.cpp
// Date: 24.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: An object drawn as constantly changing points
///////////////////////////////////////////////////////////////////////////////

#include "VulkanDynamicPointRenderObject.h"

#include "VulkanVertexCubeShader.h"
#include "VulkanArray3DShader.h"
#include "VukanTwoVertexCubeShader.h"
#include "VulkanGrid2DShader.h"


// needs explicit instantiation for every used shader

// Vertex Cube Shader
template DynamicPointRenderObject<VertexCubeShader<VertexCubeShaderType::Pong>>::uPtr 
		 DynamicPointRenderObject<VertexCubeShader<VertexCubeShaderType::Pong>>::createVulkan();

template DynamicPointRenderObject<VertexCubeShader<VertexCubeShaderType::NoLighting>>::uPtr 
		 DynamicPointRenderObject<VertexCubeShader<VertexCubeShaderType::NoLighting>>::createVulkan();

// Two Vertex Cube Shader
template DynamicPointRenderObject<TwoVertexCubeShader>::uPtr 
		 DynamicPointRenderObject<TwoVertexCubeShader>::createVulkan();

// Array 3D Shader
template DynamicPointRenderObject<Array3DShader<eShader::Pong>>::uPtr    
		 DynamicPointRenderObject<Array3DShader<eShader::Pong>>::createVulkan();

template DynamicPointRenderObject<Array3DShader<eShader::Gouraud>>::uPtr 
		 DynamicPointRenderObject<Array3DShader<eShader::Gouraud>>::createVulkan();

template DynamicPointRenderObject<Array3DShader<eShader::Diffuse>>::uPtr 
		 DynamicPointRenderObject<Array3DShader<eShader::Diffuse>>::createVulkan();

template DynamicPointRenderObject<Array3DShader<eShader::Points>>::uPtr  
		 DynamicPointRenderObject<Array3DShader<eShader::Points>>::createVulkan();


// Grid 2D Shader
template DynamicPointRenderObject<Grid2DShader<Grid2DShaderType::Diffuse>>::uPtr 
		 DynamicPointRenderObject<Grid2DShader<Grid2DShaderType::Diffuse>>::createVulkan();




template<typename TShader>
typename DynamicPointRenderObject<TShader>::uPtr DynamicPointRenderObject<TShader>::createVulkan()
{
	return std::make_unique<VulkanDynamicPointRenderObject<TShader>>();
}




template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::create(VulkanParticleRenderer& engine)
{
	createDescriptorSetLayout(engine);
	createGraphicsPipeline(engine);
	createVertexBuffer(engine);
	createUniformBuffer(engine);
	createDescriptorPool(engine);
	createDescriptorSets(engine);
	createCommandBuffer(engine);
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::draw(VulkanParticleRenderer& engine)
{
	drawFrame(engine);
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::cleanup(VulkanParticleRenderer& engine)
{
	descriptorSets.clear();
	descriptorPool.reset();
	uniformBuffers.clear();
	uniformBuffersMemory.clear();
	graphicsPipeline.reset();
	pipelineLayout.reset();
}


template<typename TShader>
vk::VertexInputBindingDescription VulkanDynamicPointRenderObject<TShader>::getVertexBindingDescription()
{
	vk::VertexInputBindingDescription bindingDescription;
	bindingDescription.setBinding(0);
	bindingDescription.setStride(sizeof(TShader::Vertex));
	bindingDescription.setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescription;
}


template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createDescriptorSetLayout(VulkanParticleRenderer& engine)
{
	if (!TShader::getGeometryShaderCode().empty())
	{
		engine.createDescriptorSetLayout(descriptorSetLayout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eGeometry);
	}
	else
	{
		engine.createDescriptorSetLayout(descriptorSetLayout, vk::ShaderStageFlagBits::eVertex);
	}
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createGraphicsPipeline(VulkanParticleRenderer& engine)
{
	engine.createGraphicsPipeline(pipelineLayout, graphicsPipeline, 
		TShader::getVertexShaderCode(), TShader::getGeometryShaderCode(), TShader::getFragmentShaderCode(),
				descriptorSetLayout, getVertexBindingDescription(), TShader::getVertexAttributeDescriptions());
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createVertexBuffer(VulkanParticleRenderer& engine)
{
	vk::DeviceSize const bufferSize = sizeof(TShader::Vertex) * DynamicPointRenderObject<TShader>::mVerticesSize;

	engine.createVertexBuffers(vertexBufferMemory, vertexBuffers, bufferSize);
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createUniformBuffer(VulkanParticleRenderer& engine)
{
	engine.createUniformBuffers(uniformBuffersMemory, uniformBuffers, sizeof(TShader::UniformBufferObject));
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createDescriptorPool(VulkanParticleRenderer& engine)
{
	engine.createDescriptorPool(descriptorPool);
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createDescriptorSets(VulkanParticleRenderer& engine)
{
	engine.createDescriptorSets(descriptorPool, descriptorSets, descriptorSetLayout, uniformBuffers, sizeof(TShader::UniformBufferObject));
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createCommandBuffer(VulkanParticleRenderer& engine)
{
	engine.recordCommands(pipelineLayout, 
		graphicsPipeline,
		vertexBuffers, 
		descriptorSets, 
		DynamicPointRenderObject<TShader>::mVerticesSize);
}


template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::drawFrame(VulkanParticleRenderer& engine)
{
	auto ubo = DynamicPointRenderObject<TShader>::mUbo;
	ubo.model = glm::translate(glm::mat4(1.0f), DynamicPointRenderObject<TShader>::mPos);

	if (DynamicPointRenderObject<TShader>::mUboFunc) {
		DynamicPointRenderObject<TShader>::mUboFunc(ubo);
	}

	engine.updateUniformBuffer(uniformBuffersMemory, ubo);

	engine.updateVertexBuffer<DynamicPointRenderObject<TShader>::Vertex>(vertexBufferMemory, 
		DynamicPointRenderObject<TShader>::mVerticesFunc, DynamicPointRenderObject<TShader>::mVerticesSize);
}



