//
// @file: render_engine_interface.h
// @author: GrandChris
// @date: 26.06.2021
// @brief: Exports the function required to control the render engine
//

#pragma once

// #include <vulkan/vulkan.hpp>
#include <span>

class RenderEngineInterface
{
public:

	virtual vk::Device & getDevice() = 0;

	virtual size_t getSwapChainSize() const = 0;

	virtual vk::Extent2D getSwapChainExtent() const = 0;

	virtual void createBuffer(vk::DeviceSize const size, 
		vk::BufferUsageFlags const usage, vk::MemoryPropertyFlags const properties, 
		vk::UniqueBuffer & buffer, vk::UniqueDeviceMemory & bufferMemory) = 0;

	virtual vk::UniqueShaderModule createShaderModule(std::vector<char> const & code) = 0;

	virtual vk::SampleCountFlagBits getMaxMsaaSamples() const = 0;

	virtual vk::RenderPass const & getRenderPass() const = 0;

	virtual std::vector<vk::UniqueCommandBuffer> & getCommandBuffers() = 0;

 
	virtual void createDescriptorSetLayout(vk::UniqueDescriptorSetLayout& descriptorSetLayout, 
			vk::ShaderStageFlags const shaderStageFlag = vk::ShaderStageFlagBits::eVertex,
			bool const useStorageBuffer = false) = 0;

	virtual	void createGraphicsPipeline(vk::UniquePipelineLayout& pipelineLayout,
			vk::UniquePipeline& graphicsPipeline,
			std::vector<char> const& vertShaderCode, std::vector<char> const& geomShaderCode, std::vector<char> const& fragShaderCode,
			vk::UniqueDescriptorSetLayout const& descriptorSetLayout,
			vk::VertexInputBindingDescription const& bindingDescription,
			std::vector<vk::VertexInputAttributeDescription>const& attributeDescriptions,
			vk::SpecializationInfo specializationVertexInfo = {}, vk::SpecializationInfo specializationGeometryInfo = {}, vk::SpecializationInfo specializationFragmentInfo = {},
			bool const useTriangles = false, bool const alphaBlending = false) = 0;

	// virtual	void createVertexBuffer(vk::UniqueDeviceMemory & stagingBufferMemory, vk::UniqueBuffer & stagingBuffer, 
	// 		vk::UniqueDeviceMemory& vertexBufferMemory, vk::UniqueBuffer& vertexBuffer,
	// 		std::span<uint8_t> const& vertices) = 0;

	virtual void createVertexBuffers(std::vector<vk::UniqueDeviceMemory>& vertexBufferMemory,
			std::vector<vk::UniqueBuffer>& vertexBuffers, size_t const vertexBufferSize) = 0;

    virtual void createStorageBuffers(std::vector<vk::UniqueDeviceMemory>& storageBufferMemory,
			std::vector<vk::UniqueBuffer>& storageBuffers, size_t const storageBufferSize) = 0;

    virtual void createUniformBuffers(std::vector<vk::UniqueDeviceMemory>& uniformBuffersMemory,
			std::vector<vk::UniqueBuffer>& uniformBuffers, size_t const UniformBufferObjectSize) = 0;

    virtual void createDescriptorPool(vk::UniqueDescriptorPool& descriptorPool) = 0;

    virtual void createDescriptorSets(vk::UniqueDescriptorPool& descriptorPool, std::vector<vk::DescriptorSet> & descriptorSets,
				vk::UniqueDescriptorSetLayout const& descriptorSetLayout,
				std::vector<vk::UniqueBuffer> const & uniformBuffers, size_t const UniformBufferObjectSize, 
				std::vector<vk::UniqueBuffer> const& storageBuffers = std::vector<vk::UniqueBuffer>(), size_t const StorageBufferObjectSize = 0) = 0;

    // virtual void recordCommands(vk::UniquePipelineLayout const& pipelineLayout,
	// 		vk::UniquePipeline const& graphicsPipeline,
	// 		vk::UniqueBuffer const& vertexBuffer,
	// 		std::vector<vk::DescriptorSet> const& descriptorSets,
	// 		size_t const verticesCount) = 0;

	virtual void recordCommands(vk::UniquePipelineLayout const& pipelineLayout, 
		vk::UniquePipeline const& graphicsPipeline,
		std::vector<vk::UniqueBuffer> const& vertexBuffers,
		std::vector<vk::DescriptorSet> const& descriptorSets,
		size_t const verticesCount, 
		char const * name) = 0;

    virtual void updateUniformBuffer(std::vector<vk::UniqueDeviceMemory> const& uniformBuffersMemory,
			std::span<uint8_t> const& uniformBufferObject) = 0;

    // virtual void updateVertexBuffer(std::vector<vk::UniqueDeviceMemory> const& vertexBufferMemory,
	// 		std::span<uint8_t> const& vertices) = 0;

	virtual void updateVertexBuffer(std::vector<vk::UniqueDeviceMemory> const& vertexBufferMemory,
		size_t const size, std::function<void(void *, size_t)> func) = 0;

};




