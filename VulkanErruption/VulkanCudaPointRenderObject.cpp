///////////////////////////////////////////////////////////////////////////////
// File: VulkanCudaPointRenderObject.cpp
// Date: 19.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: An object drawn as static points
///////////////////////////////////////////////////////////////////////////////

#include "VulkanCudaPointRenderObject.h"

#include "WindowsSecurityAttributes.h"


#include <windows.h>
#include <vulkan/vulkan_win32.h>

// runtime loaded vulkan functions

VKAPI_ATTR VkResult VKAPI_CALL vkGetMemoryWin32HandleKHR(
	VkInstance instance,
	VkDevice device, 
	const VkMemoryGetWin32HandleInfoKHR* pGetWin32HandleInfo, 
	void ** pHandle)
{
	auto func = (PFN_vkGetMemoryWin32HandleKHR)vkGetInstanceProcAddr(instance, "vkGetMemoryWin32HandleKHR");
	if (func != nullptr) {
		return func(device, pGetWin32HandleInfo, pHandle);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

// <<---


CudaPointRenderObject::uPtr CudaPointRenderObject::createVulkan()
{
	return std::make_unique<VulkanCudaPointRenderObject>();
}


void VulkanCudaPointRenderObject::create(VulkanParticleRenderer& engine)
{
	createGraphicsPipeline(engine);
	createVertexBuffer(engine);
	createUniformBuffer(engine);
	createDescriptorSets(engine);
	createCommandBuffer(engine);
}

void VulkanCudaPointRenderObject::draw(VulkanParticleRenderer& engine)
{
	drawFrame(engine);
}

void VulkanCudaPointRenderObject::cleanup(VulkanParticleRenderer& engine)
{
	descriptorSets.clear();
	commandBuffers.clear();
	uniformBuffers.clear();
	uniformBuffersMemory.clear();
	graphicsPipeline.reset();
	pipelineLayout.reset();
}



vk::VertexInputBindingDescription VulkanCudaPointRenderObject::getVertexBindingDescription()
{
	vk::VertexInputBindingDescription bindingDescription;
	bindingDescription.setBinding(0);
	bindingDescription.setStride(sizeof(Vertex));
	bindingDescription.setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescription;
}

std::vector<vk::VertexInputAttributeDescription> VulkanCudaPointRenderObject::getVertexAttributeDescriptions()
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


void VulkanCudaPointRenderObject::createGraphicsPipeline(VulkanParticleRenderer& engine)
{
	engine.createGraphicsPipeline(pipelineLayout, graphicsPipeline, vertShaderCode, fragShaderCode,
		getVertexBindingDescription(), getVertexAttributeDescriptions());
}



void VulkanCudaPointRenderObject::createVertexBuffer(VulkanParticleRenderer& engine)
{
	createVertexBufferExtMem(engine, vertexBufferMemory, vertexBuffer, mExternalVertexBufferSize * sizeof(Vertex));
	void* handle = getExternalVertexBufferHandle(engine);

	assert(mInitExternalVertexBufferFunction != nullptr);
	if (mInitExternalVertexBufferFunction != nullptr)
	{
		// call extern function for cuda to initialize
		mInitExternalVertexBufferFunction(handle, mExternalVertexBufferSize);
	}

	mVerticesFunc(true);	// call a function to update vertices through a cuda kernel
}

void* VulkanCudaPointRenderObject::getExternalVertexBufferHandle(VulkanParticleRenderer& engine)
{
	// get external memory pointer
	VkMemoryGetWin32HandleInfoKHR vkMemoryGetWin32HandleInfoKHR = {};
	vkMemoryGetWin32HandleInfoKHR.sType = VK_STRUCTURE_TYPE_MEMORY_GET_WIN32_HANDLE_INFO_KHR;
	vkMemoryGetWin32HandleInfoKHR.pNext = NULL;
	vkMemoryGetWin32HandleInfoKHR.memory = vertexBufferMemory.get();
	vkMemoryGetWin32HandleInfoKHR.handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT;

	void* handle = nullptr;
	auto const res = vkGetMemoryWin32HandleKHR(engine.instance.get(), engine.device.get(),
		&vkMemoryGetWin32HandleInfoKHR, &handle);

	assert(res == VK_SUCCESS);

	return handle;	
}

void VulkanCudaPointRenderObject::createVertexBufferExtMem(VulkanParticleRenderer& engine, 
	vk::UniqueDeviceMemory& vertexBufferMemory, vk::UniqueBuffer& vertexBuffer, 
	size_t const size)
{
	vk::DeviceSize const bufferSize = size;

	createBufferExtMem(engine, bufferSize, vk::BufferUsageFlagBits::eVertexBuffer,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		vertexBuffer, vertexBufferMemory
	);
}

#define	DXGI_SHARED_RESOURCE_READ	( 0x80000000L )
#define	DXGI_SHARED_RESOURCE_WRITE	( 1 )

void VulkanCudaPointRenderObject::createBufferExtMem(VulkanParticleRenderer& engine, 
	vk::DeviceSize const size,	vk::BufferUsageFlags const usage, 
	vk::MemoryPropertyFlags const properties, 
	vk::UniqueBuffer& buffer, vk::UniqueDeviceMemory& bufferMemory)
{
	vk::BufferCreateInfo bufferInfo;
	bufferInfo.setSize(size);
	bufferInfo.setUsage(usage);
	bufferInfo.setSharingMode(vk::SharingMode::eExclusive);

	buffer = engine.device->createBufferUnique(bufferInfo);

	auto const memRequirements = engine.device->getBufferMemoryRequirements(buffer.get());

	// external memory options
	WindowsSecurityAttributes winSecurityAttributes;

	VkExportMemoryWin32HandleInfoKHR vulkanExportMemoryWin32HandleInfoKHR = {};
	vulkanExportMemoryWin32HandleInfoKHR.sType = VK_STRUCTURE_TYPE_EXPORT_MEMORY_WIN32_HANDLE_INFO_KHR;
	vulkanExportMemoryWin32HandleInfoKHR.pNext = NULL;
	vulkanExportMemoryWin32HandleInfoKHR.pAttributes = &winSecurityAttributes;
	vulkanExportMemoryWin32HandleInfoKHR.dwAccess = DXGI_SHARED_RESOURCE_READ | DXGI_SHARED_RESOURCE_WRITE;
	vulkanExportMemoryWin32HandleInfoKHR.name = (LPCWSTR)NULL;

	VkExportMemoryAllocateInfoKHR vulkanExportMemoryAllocateInfoKHR = {};
	vulkanExportMemoryAllocateInfoKHR.sType = VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO_KHR;

	vulkanExportMemoryAllocateInfoKHR.pNext = &vulkanExportMemoryWin32HandleInfoKHR;
	vulkanExportMemoryAllocateInfoKHR.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_OPAQUE_WIN32_BIT;
	// <---

	vk::MemoryAllocateInfo allocInfo;
	allocInfo.setPNext(&vulkanExportMemoryAllocateInfoKHR);
	allocInfo.setAllocationSize(memRequirements.size);	
	allocInfo.setMemoryTypeIndex(engine.findMemoryType(memRequirements.memoryTypeBits, properties));

	bufferMemory = engine.device->allocateMemoryUnique(allocInfo);

	engine.device->bindBufferMemory(buffer.get(), bufferMemory.get(), 0);
}

void VulkanCudaPointRenderObject::createUniformBuffer(VulkanParticleRenderer& engine)
{
	engine.createUniformBuffers(uniformBuffersMemory, uniformBuffers, sizeof(UniformBufferObject));
}

void VulkanCudaPointRenderObject::createDescriptorSets(VulkanParticleRenderer& engine)
{
	engine.createDescriptorSets(descriptorSets, uniformBuffers, sizeof(UniformBufferObject));
}

void VulkanCudaPointRenderObject::createCommandBuffer(VulkanParticleRenderer& engine)
{
	engine.createCommandBuffers(commandBuffers, pipelineLayout, graphicsPipeline,
		vertexBuffer, descriptorSets, mExternalVertexBufferSize);
}



void VulkanCudaPointRenderObject::drawFrame(VulkanParticleRenderer& engine)
{
	mVerticesFunc(false);	// call a function to update vertices through a cuda kernel

	UniformBufferObject ubo;
	ubo.view = glm::lookAt(mPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	engine.drawFrame(commandBuffers, uniformBuffersMemory, ubo);
}

