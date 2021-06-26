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
#include "VulkanGrid2DLightingShader.h"


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
template DynamicPointRenderObject<Grid2DShader>::uPtr 
		 DynamicPointRenderObject<Grid2DShader>::createVulkan();

template DynamicPointRenderObject<Grid2DLightingShader>::uPtr
	DynamicPointRenderObject<Grid2DLightingShader>::createVulkan();




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
	createStorageBuffer(engine);
	createUniformBuffer(engine);
	createDescriptorPool(engine);
	createDescriptorSets(engine);
	createCommandBuffer(engine);

	fullBufferUpdateRequired = true;
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

	// need not be deleted?
	uniformBuffers.clear();
	uniformBuffersMemory.clear();
	storageBuffers.clear();
	storageBufferMemory.clear();
	vertexBuffers.clear();
	vertexBufferMemory.clear();

	graphicsPipeline.reset();
	pipelineLayout.reset();
}


template<typename TShader>
vk::VertexInputBindingDescription VulkanDynamicPointRenderObject<TShader>::getVertexBindingDescription()
{
	vk::VertexInputBindingDescription bindingDescription;
	bindingDescription.setBinding(0);
	bindingDescription.setStride(sizeof(TShader::VertexBufferElement));
	bindingDescription.setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescription;
}


template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createDescriptorSetLayout(VulkanParticleRenderer& engine)
{
	if (!TShader::getGeometryShaderCode().empty())
	{
		engine.createDescriptorSetLayout(descriptorSetLayout, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eGeometry, 
			DynamicPointRenderObject<TShader>::mStorageBufferSize > 0);
	}
	else
	{
		engine.createDescriptorSetLayout(descriptorSetLayout, vk::ShaderStageFlagBits::eVertex, 
			DynamicPointRenderObject<TShader>::mStorageBufferSize > 0);
	}
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createGraphicsPipeline(VulkanParticleRenderer& engine)
{	
	// vertex shader specialization info
	auto const vertexInfoMap = TShader::getSpecializationInfoVertexShader();
	auto vertexInfoObj = DynamicPointRenderObject<TShader>::mSpecializationInfoVertexShader;

	vk::SpecializationInfo speciaVertexInfo = {};
	if (!vertexInfoMap.empty())
	{
		speciaVertexInfo.setMapEntryCount(static_cast<uint32_t>(vertexInfoMap.size()));
		speciaVertexInfo.setPMapEntries(vertexInfoMap.data());
		speciaVertexInfo.setDataSize(sizeof(TShader::SpecializationInfoVertexShader));
		speciaVertexInfo.setPData(&vertexInfoObj);
	}

	// geometry shader specialization info
	auto const geomInfoMap = TShader::getSpecializationInfoGeometryShader();
	auto geominfoObj = DynamicPointRenderObject<TShader>::mSpecializationInfoGeometryShader;

	vk::SpecializationInfo specialGeometryInfo = {};
	if (!geomInfoMap.empty())
	{
		specialGeometryInfo.setMapEntryCount(static_cast<uint32_t>(geomInfoMap.size()));
		specialGeometryInfo.setPMapEntries(geomInfoMap.data());
		specialGeometryInfo.setDataSize(sizeof(TShader::SpecializationInfoGeometryShader));
		specialGeometryInfo.setPData(&geominfoObj);
	}

	// fragment shader specialization info
	auto const fragInfoMap = TShader::getSpecializationInfoFragmentShader();
	auto fraginfoObj = DynamicPointRenderObject<TShader>::mSpecializationInfoFragmentShader;

	vk::SpecializationInfo SpecialFragmentInfo = {};
	if (!fragInfoMap.empty())
	{
		SpecialFragmentInfo.setMapEntryCount(static_cast<uint32_t>(fragInfoMap.size()));
		SpecialFragmentInfo.setPMapEntries(fragInfoMap.data());
		SpecialFragmentInfo.setDataSize(sizeof(TShader::SpecializationInfoFragmentShader));
		SpecialFragmentInfo.setPData(&fraginfoObj);
	}

	engine.createGraphicsPipeline(pipelineLayout, graphicsPipeline, 
		TShader::getVertexShaderCode(), TShader::getGeometryShaderCode(), TShader::getFragmentShaderCode(),
				descriptorSetLayout, getVertexBindingDescription(), TShader::getVertexAttributeDescriptions(), 
		speciaVertexInfo, specialGeometryInfo, SpecialFragmentInfo);
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createVertexBuffer(VulkanParticleRenderer& engine)
{
	if (DynamicPointRenderObject<TShader>::mVertexBufferSize > 0)
	{
		vk::DeviceSize const bufferSize = sizeof(TShader::VertexBufferElement) * DynamicPointRenderObject<TShader>::mVertexBufferSize;

		engine.createVertexBuffers(vertexBufferMemory, vertexBuffers, bufferSize);
	}
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createStorageBuffer(VulkanParticleRenderer& engine)
{
	if (DynamicPointRenderObject<TShader>::mStorageBufferSize > 0)
	{
		vk::DeviceSize const bufferSize = sizeof(TShader::StorageBufferElement) * DynamicPointRenderObject<TShader>::mStorageBufferSize;

		engine.createStorageBuffers(storageBufferMemory, storageBuffers, bufferSize);
	}
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
	engine.createDescriptorSets(descriptorPool, descriptorSets, descriptorSetLayout, 
		uniformBuffers, sizeof(TShader::UniformBufferObject),
		storageBuffers, sizeof(TShader::StorageBufferElement) * DynamicPointRenderObject<TShader>::mStorageBufferSize
		);
}

template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::createCommandBuffer(VulkanParticleRenderer& engine)
{
	engine.recordCommands(pipelineLayout, 
		graphicsPipeline,
		vertexBuffers, 
		descriptorSets, 
		DynamicPointRenderObject<TShader>::mVertexBufferSize,
		DynamicPointRenderObject<TShader>::mName);
}


template<typename TShader>
void VulkanDynamicPointRenderObject<TShader>::drawFrame(VulkanParticleRenderer& engine)
{
	// update uniform buffer
	auto ubo = DynamicPointRenderObject<TShader>::mUbo;
	ubo.model = glm::translate(glm::mat4(1.0f), DynamicPointRenderObject<TShader>::mPos);

	if (DynamicPointRenderObject<TShader>::mUboFunc) {
		DynamicPointRenderObject<TShader>::mUboFunc(ubo);
	}

	engine.updateUniformBuffer(uniformBuffersMemory, ubo);

	// update vertex buffer
	engine.updateVertexBuffer<DynamicPointRenderObject<TShader>::VertexBufferElement>(vertexBufferMemory, 
		DynamicPointRenderObject<TShader>::mVerticesFunc, DynamicPointRenderObject<TShader>::mVertexBufferSize, fullBufferUpdateRequired);
	

	// update storage buffer
	if (DynamicPointRenderObject<TShader>::mStorageBufferSize > 0)
	{
		engine.updateVertexBuffer<DynamicPointRenderObject<TShader>::StorageBufferElement>(storageBufferMemory,
			DynamicPointRenderObject<TShader>::mSBOFunc, DynamicPointRenderObject<TShader>::mStorageBufferSize, fullBufferUpdateRequired);
	}

	fullBufferUpdateRequired = false;
}



