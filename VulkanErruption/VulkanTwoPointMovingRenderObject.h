///////////////////////////////////////////////////////////////////////////////
// File: VulkanTwoPointMovingRenderObject.h
// Date: 19.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: An object drawn as static points
///////////////////////////////////////////////////////////////////////////////

#pragma once


#include "TwoPointMovingRenderObject.h"

#include "VulkanParticleRenderer.h"


#include "shaders/index_logo_vert_spv.h"
#include "shaders/frag_spv.h"


class VulkanTwoPointMovingRenderObject : public TwoPointMovingRenderObject
{
public:
	// Inherited from RenderObject
	virtual void create(VulkanParticleRenderer& engine) override;
	virtual void draw(VulkanParticleRenderer& engine) override;
	virtual void cleanup(VulkanParticleRenderer& engine) override;


private:
	inline static std::vector<char> const vertShaderCode = index_logo_vert_spv;
	inline static std::vector<char> const fragShaderCode = frag_spv;

	// Description of a Vertex
	static vk::VertexInputBindingDescription getVertexBindingDescription();
	static std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions();

	// Create functions

	void createGraphicsPipeline(VulkanParticleRenderer& engine);

	void createVertexBuffer(VulkanParticleRenderer& engine);

	void createUniformBuffer(VulkanParticleRenderer& engine);

	void createDescriptorSets(VulkanParticleRenderer& engine);

	void createCommandBuffer(VulkanParticleRenderer& engine);


	// Draw functions
	void drawFrame(VulkanParticleRenderer& engine);


	// Graphics pipeline
	vk::UniquePipelineLayout pipelineLayout;
	vk::UniquePipeline graphicsPipeline;

	// Vertex Buffer
	vk::UniqueDeviceMemory vertexBufferMemory;
	vk::UniqueBuffer vertexBuffer;

	// Uniform Buffers
	std::vector<vk::UniqueDeviceMemory> uniformBuffersMemory;
	std::vector<vk::UniqueBuffer> uniformBuffers;

	// Command Buffers
	std::vector<vk::UniqueCommandBuffer> commandBuffers;

	// Descriptor sets
	std::vector<vk::DescriptorSet> descriptorSets;


};

