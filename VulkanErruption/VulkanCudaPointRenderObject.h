///////////////////////////////////////////////////////////////////////////////
// File: VulkanCudaPointRenderObject.h
// Date: 19.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: An object drawn as static points
///////////////////////////////////////////////////////////////////////////////

#pragma once


#include "CudaPointRenderObject.h"


#include "VulkanParticleRenderer.h"


#include "shaders/generated/vec3_vert_spv.h"
#include "shaders/generated/shader_frag_spv.h"


class VulkanCudaPointRenderObject : public CudaPointRenderObject
{
public:
	// Inherited from RenderObject
	virtual void create(VulkanParticleRenderer& engine) override;
	virtual void draw(VulkanParticleRenderer& engine) override;
	virtual void cleanup(VulkanParticleRenderer& engine) override;


private:
	inline static std::vector<char> const vertShaderCode = vec3_vert_spv;
	inline static std::vector<char> const fragShaderCode = shader_frag_spv;

	// Description of a Vertex
	static vk::VertexInputBindingDescription getVertexBindingDescription();
	static std::vector<vk::VertexInputAttributeDescription> getVertexAttributeDescriptions();

	// Create functions

	void createDescriptorSetLayout(VulkanParticleRenderer& engine);

	void createGraphicsPipeline(VulkanParticleRenderer& engine);

	void createVertexBuffer(VulkanParticleRenderer& engine);

		void* getExternalVertexBufferHandle(VulkanParticleRenderer& engine);

		void createVertexBufferExtMem(VulkanParticleRenderer& engine, vk::UniqueDeviceMemory& vertexBufferMemory, vk::UniqueBuffer& vertexBuffer,
			size_t const bufferSize);

		void createBufferExtMem(VulkanParticleRenderer& engine, vk::DeviceSize const size,
			vk::BufferUsageFlags const usage, vk::MemoryPropertyFlags const properties,
			vk::UniqueBuffer& buffer, vk::UniqueDeviceMemory& bufferMemory);

	void createUniformBuffer(VulkanParticleRenderer& engine);

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


