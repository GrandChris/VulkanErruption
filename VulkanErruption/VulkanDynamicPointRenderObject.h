///////////////////////////////////////////////////////////////////////////////
// File: VulkanDynamicPointRenderObject.h
// Date: 19.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: An object drawn as static points
///////////////////////////////////////////////////////////////////////////////

#pragma once


#include "DynamicPointRenderObject.h"

#include "VulkanParticleRenderer.h"


template<typename TShader>
class VulkanDynamicPointRenderObject : public DynamicPointRenderObject<TShader>
{
public:
	// Inherited from RenderObject
	virtual void create(VulkanParticleRenderer& engine) override;
	virtual void draw(VulkanParticleRenderer& engine) override;
	virtual void cleanup(VulkanParticleRenderer& engine) override;

private:

	// Description of a Vertex
	static vk::VertexInputBindingDescription getVertexBindingDescription();

	// Create functions

	void createDescriptorSetLayout(VulkanParticleRenderer& engine);

	void createGraphicsPipeline(VulkanParticleRenderer& engine);

	void createVertexBuffer(VulkanParticleRenderer& engine);

	void createUniformBuffer(VulkanParticleRenderer& engine);

	void createDescriptorPool(VulkanParticleRenderer& engine);

	void createDescriptorSets(VulkanParticleRenderer& engine);

	void createCommandBuffer(VulkanParticleRenderer& engine);


	// Draw functions
	void drawFrame(VulkanParticleRenderer& engine);

	// Uniform Buffer description
	vk::UniqueDescriptorSetLayout descriptorSetLayout;

	// Graphics pipeline
	vk::UniquePipelineLayout pipelineLayout;
	vk::UniquePipeline graphicsPipeline;

	// Vertex Buffer
	std::vector<vk::UniqueDeviceMemory> vertexBufferMemory;
	std::vector<vk::UniqueBuffer> vertexBuffers;

	// Uniform Buffers
	std::vector<vk::UniqueDeviceMemory> uniformBuffersMemory;
	std::vector<vk::UniqueBuffer> uniformBuffers;

	// Descriptor Pool
	vk::UniqueDescriptorPool descriptorPool;

	// Descriptor sets
	std::vector<vk::DescriptorSet> descriptorSets;
};


