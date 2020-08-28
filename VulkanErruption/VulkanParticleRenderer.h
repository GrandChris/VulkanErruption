///////////////////////////////////////////////////////////////////////////////
// File: VulkanParticleRenderer.h
// Date: 13.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Drawing particles with Vulkan
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ParticleRenderer.h"

//#include <Windows.h>

//

#include <Windows.h>
#include <vulkan/vulkan.hpp>

//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



#include <glm/glm.hpp>

#include "upGLFWWindow.h"
#include "glfwFPS.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <memory>
#include <optional>


class VulkanParticleRenderer : public ParticleRenderer {
public:
	// C-Tor
	VulkanParticleRenderer(bool enableValidationLayer);

	// D-Tor
	~VulkanParticleRenderer();

	// Geerbt �ber ParticleRenderer
	//virtual void doDraw(std::vector<Vertex> const & vertices) override;

	// Geerbt �ber ParticleRenderer
	virtual void create(RenderObject::uPtr const& obj) override;
	virtual void draw(RenderObject::uPtr const& obj) override;
	virtual void cleanup(RenderObject::uPtr const& obj) override;

	virtual void run() override;

public:
	struct QueueFamilyIndices;
	struct SwapChainSupportDetails;

	void initWindow();

	void initVulkan();

		void createInstance();

			std::vector<const char*> getRequiredExtensions();

			bool checkValidationLayerSupport();

		void setupDebugMessenger();

			void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo);

		void createSurface();

		void pickPhysicalDevice();

			int rateDeviceSuitability(vk::PhysicalDevice const & device);

			bool isDeviceSuitable(vk::PhysicalDevice const & device);

			QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice const& device);

			vk::SampleCountFlagBits getMaxUsableSampleCount();

		void createLogicalDevice();

		bool checkDeviceExtensionSupport(vk::PhysicalDevice const& device);

		void createSwapChain();

			SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice const & device);

			vk::SurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const & availableFormats);

			vk::PresentModeKHR chooseSwapPresentMode(std::vector<vk::PresentModeKHR> const & availablePresentModes);

			vk::Extent2D chooseSwapExtent(vk::SurfaceCapabilitiesKHR const& capabilities);

		void createImageViews();

		void createRenderPass();

		void createDescriptorSetLayout(vk::UniqueDescriptorSetLayout& descriptorSetLayout, 
			vk::ShaderStageFlags const shaderStageFlag = vk::ShaderStageFlagBits::eVertex,
			bool const useStorageBuffer = false);

		void createGraphicsPipeline(vk::UniquePipelineLayout & pipelineLayout, 
			vk::UniquePipeline & graphicsPipeline,
			std::vector<char> const & vertShaderCode, std::vector<char> const & fragShaderCode,
			vk::UniqueDescriptorSetLayout const& descriptorSetLayout,
			vk::VertexInputBindingDescription const & bindingDescription,
			std::vector<vk::VertexInputAttributeDescription>const & attributeDescriptions,
			vk::SpecializationInfo specializationVertexInfo = {}, vk::SpecializationInfo specializationGeometryInfo = {}, vk::SpecializationInfo specializationFragmentInfo = {},
			bool const useTriangles = false);

		void createGraphicsPipeline(vk::UniquePipelineLayout& pipelineLayout,
			vk::UniquePipeline& graphicsPipeline,
			std::vector<char> const& vertShaderCode, std::vector<char> const& geomShaderCode, std::vector<char> const& fragShaderCode,
			vk::UniqueDescriptorSetLayout const& descriptorSetLayout,
			vk::VertexInputBindingDescription const& bindingDescription,
			std::vector<vk::VertexInputAttributeDescription>const& attributeDescriptions,
			vk::SpecializationInfo specializationVertexInfo = {}, vk::SpecializationInfo specializationGeometryInfo = {}, vk::SpecializationInfo specializationFragmentInfo = {},
			bool const useTriangles = false, bool const alphaBlending = false);




		//void createGraphicsPipeline();

			vk::UniqueShaderModule createShaderModule(std::vector<char> const & code);

		void createFramebuffers();

		void createCommandPool();

		void createColorResources();

		void createDepthResources();

			vk::Format findDepthFormat();

				vk::Format findSupportedFormat(std::vector<vk::Format> const& candidates,
					vk::ImageTiling tiling, vk::FormatFeatureFlags features);

			bool hasStencilComponent(vk::Format format);

			void createImage(uint32_t const width, uint32_t const height, vk::SampleCountFlagBits const numSample,
				vk::Format const & format,
				vk::ImageTiling const & tiling, vk::ImageUsageFlags const & usage, 
				vk::MemoryPropertyFlags const & properties,
				vk::UniqueImage& image, vk::UniqueDeviceMemory & imageMemory);

			vk::UniqueImageView createImageView(vk::Image const& image, vk::Format const& format,
				vk::ImageAspectFlags const& aspectFlags);

		template<typename T>
		void createVertexBuffer(vk::UniqueDeviceMemory & vertexBufferMemory, vk::UniqueBuffer & vertexBuffer,
			std::vector<T> const& vertices);

		template<typename T>
		void createVertexBuffer(vk::UniqueDeviceMemory & stagingBufferMemory, vk::UniqueBuffer & stagingBuffer, 
			vk::UniqueDeviceMemory& vertexBufferMemory, vk::UniqueBuffer& vertexBuffer,
			std::vector<T> const& vertices);
		//void createVertexBuffer();

		void createBuffers(std::vector<vk::UniqueDeviceMemory>& bufferMemory,
			std::vector<vk::UniqueBuffer>& buffers, size_t const bufferSize, vk::BufferUsageFlagBits usageFlags);

		void createVertexBuffers(std::vector<vk::UniqueDeviceMemory>& vertexBufferMemory,
			std::vector<vk::UniqueBuffer>& vertexBuffers, size_t const vertexBufferSize);

		void createStorageBuffers(std::vector<vk::UniqueDeviceMemory>& storageBufferMemory,
			std::vector<vk::UniqueBuffer>& storageBuffers, size_t const storageBufferSize);
			
			uint32_t findMemoryType(uint32_t const typeFilter, vk::MemoryPropertyFlags const & properties);

			void createBuffer(vk::DeviceSize const size, vk::BufferUsageFlags const usage,
				vk::MemoryPropertyFlags const properties, vk::UniqueBuffer& buffer, vk::UniqueDeviceMemory& bufferMemory);

			void copyBuffer(vk::Buffer const & srcBuffer, vk::Buffer & dstBuffer, vk::DeviceSize size);

		void createUniformBuffers(std::vector<vk::UniqueDeviceMemory>& uniformBuffersMemory,
				std::vector<vk::UniqueBuffer>& uniformBuffers, size_t const UniformBufferObjectSize);
		//void createUniformBuffers();

			void createDescriptorPool(vk::UniqueDescriptorPool& descriptorPool);

			void createDescriptorSets(vk::UniqueDescriptorPool& descriptorPool, std::vector<vk::DescriptorSet> & descriptorSets,
				vk::UniqueDescriptorSetLayout const& descriptorSetLayout,
				std::vector<vk::UniqueBuffer> const & uniformBuffers, size_t const UniformBufferObjectSize, 
				std::vector<vk::UniqueBuffer> const& storageBuffers = std::vector<vk::UniqueBuffer>(), size_t const StorageBufferObjectSize = 0);
			//void createDescriptorSets();

		void createCommandBuffersBegin();
		void createCommandBuffersEnd();

		void recordCommands(vk::UniquePipelineLayout const& pipelineLayout,
			vk::UniquePipeline const& graphicsPipeline,
			vk::UniqueBuffer const& vertexBuffer,
			std::vector<vk::DescriptorSet> const& descriptorSets,
			size_t const verticesCount);

		void recordCommands(vk::UniquePipelineLayout const& pipelineLayout,
			vk::UniquePipeline const& graphicsPipeline,
			std::vector<vk::UniqueBuffer> const& vertexBuffers,
			std::vector<vk::DescriptorSet> const& descriptorSets,
			size_t const verticesCount,
			char const* name);

		void createSyncObjects();

	void mainLoop();

		void drawFrameStart();
		void drawFrameEnd();


			template<typename T>
			void updateUniformBuffer(std::vector<vk::UniqueDeviceMemory> const& uniformBuffersMemory,
				T const& uniformBufferObject);
			//void updateUniformBuffer(uint32_t currentImage);

			template<typename T>
			void updateVertexBuffer(vk::UniqueDeviceMemory const & vertexBufferMemory,
				std::vector<T> const& vertices);


			template<typename T>
			void updateVertexBuffer(std::vector<vk::UniqueDeviceMemory> const& vertexBufferMemory,
				std::vector<T> const& vertices);

			template<typename T, typename TFunc>
			void updateVertexBuffer(std::vector<vk::UniqueDeviceMemory> const& vertexBufferMemory,
				TFunc & func, size_t const size, bool const fullVertexBufferUpdateRequired);

	void cleanup();

	void recreateSwapChain();

	void cleanupSwapChain();

	// Instance
	upGLFWWindow window;

	vk::UniqueInstance instance;

	// Validation layers

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

//#ifdef NDEBUG
	const bool enableValidationLayers = true;
//#else
//	const bool enableValidationLayers = true;
//#endif

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	vk::UniqueDebugUtilsMessengerEXT debugMessanger;

	// Physical devices and queue families

	vk::PhysicalDevice physicalDevice;

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() const {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	// Logical device and queues

	vk::UniqueDevice device;

	vk::Queue graphicsQueue;

	// Window surface

	vk::UniqueSurfaceKHR surface;

	vk::Queue presentQueue;

	// Swap chain

	std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		// Cuda extensions
		VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME,
		"VK_KHR_external_memory_win32"
	};

	struct SwapChainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};

	vk::UniqueSwapchainKHR swapChain;

	std::vector<vk::Image> swapChainImages;
	vk::Format swapChainImageFormat;
	vk::Extent2D swapChainExtent;

	// Image views

	std::vector<vk::UniqueImageView> swapChainImageViews;

	// Shader modules

	// Render passes

	vk::UniqueRenderPass renderPass;

	// Fixed functions

	//vk::UniquePipelineLayout pipelineLayout;

	// Graphics pipeline

	//vk::UniquePipeline graphicsPipeline;

	// Framebuffers

	std::vector<vk::UniqueFramebuffer> swapChainFramebuffers;

	// Command buffers

	vk::UniqueCommandPool commandPool;

	std::vector<vk::UniqueCommandBuffer> commandBuffers;

	// Rendering and presentation

	int const MAX_FRAMES_IN_FLIGHT = 2;

	std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
	std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
	std::vector<vk::UniqueFence> inFlightFences;

	size_t currentFrame = 0;
	vk::ResultValue<uint32_t> currentImageResultValue = vk::ResultValue<uint32_t>(vk::Result::eSuccess, 0);
	//uint32_t currentImageIndex = 0;

	// Swap chain recreation

	bool framebufferResized = false;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	static void keyInputCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

	// Vertex input description

	//std::vector<glmVertex> vertices =
	//{
	//	{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
	//	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	//	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	//};

	// Vertex buffer creation

	//vk::UniqueDeviceMemory vertexBufferMemory;

	//vk::UniqueBuffer vertexBuffer;

	// Staging buffer

	// Descriptor layout and buffer

	//struct UniformBufferObject
	//{
	//	alignas(16) glm::mat4 model;
	//	alignas(16) glm::mat4 view;
	//	alignas(16) glm::mat4 proj;
	//};

	//vk::UniqueDescriptorSetLayout descriptorSetLayout;
	
	//std::vector<vk::UniqueDeviceMemory> uniformBuffersMemory;

	//std::vector<vk::UniqueBuffer> uniformBuffers;

	// Descriptor pool and sets
	//vk::UniqueDescriptorPool descriptorPool;

	//std::vector<vk::DescriptorSet> descriptorSets;


	// Depth buffering
	vk::UniqueDeviceMemory depthImageMemory;
	vk::UniqueImage depthImage;
	vk::UniqueImageView depthImageView;

	// MSAA
	vk::SampleCountFlagBits const msaaSamplesPrefered = vk::SampleCountFlagBits::e16;	// user defined
	//vk::SampleCountFlagBits const msaaSamplesPrefered = vk::SampleCountFlagBits::e1;	// user defined
	vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;			// aktual

	vk::UniqueDeviceMemory colorImageMemory;
	vk::UniqueImage colorImage;
	vk::UniqueImageView colorImageView;

	// Sample Shading
	bool const sampleShadingEnabled = true;
	float const smapleShadingMinFactor = 1.0f;




	// Show FPS
	glfwFPS mFPS;





	// Geerbt �ber ParticleRenderer
	virtual void setVSync(bool const vsyncEnabled) override;

	virtual void setWindowSize(size_t const widht, size_t const height, bool const fullscreenEnabled = false) override;

};


// #######+++++++ Implementation +++++++#######

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

template<typename T>
inline void VulkanParticleRenderer::createVertexBuffer(
	vk::UniqueDeviceMemory& vertexBufferMemory, vk::UniqueBuffer& vertexBuffer, 
	std::vector<T> const& vertices)
{
	vk::UniqueDeviceMemory stagingBufferMemory;
	vk::UniqueBuffer stagingBuffer;

	vk::DeviceSize const bufferSize = sizeof(vertices.front()) * vertices.size();

	createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		stagingBuffer, stagingBufferMemory
	);

	// copy vertices
	auto const data = device->mapMemory(stagingBufferMemory.get(), 0, bufferSize);
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	device->unmapMemory(stagingBufferMemory.get());

	createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		vertexBuffer, vertexBufferMemory
	);

	copyBuffer(stagingBuffer.get(), vertexBuffer.get(), bufferSize);

}



template<typename T>
inline void VulkanParticleRenderer::updateUniformBuffer(std::vector<vk::UniqueDeviceMemory> const& uniformBuffersMemory, 
	T const& uniformBufferObject)
{
	assert(!uniformBuffersMemory.empty());
	assert(device); 
	assert(currentImageResultValue.result == vk::Result::eSuccess);
	uint32_t const imageIndex = currentImageResultValue.value;

	T ubo = uniformBufferObject;

	ubo.view = glm::lookAt(mEye, mView, glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 100'000.0f);
	ubo.proj[1][1] *= -1; // invert Y for Vulkan

	auto const data = device->mapMemory(uniformBuffersMemory[imageIndex].get(), 0, sizeof(ubo));
	memcpy(data, &ubo, static_cast<size_t>(sizeof(ubo)));
	device->unmapMemory(uniformBuffersMemory[imageIndex].get());
}


template<typename T>
inline void VulkanParticleRenderer::updateVertexBuffer(vk::UniqueDeviceMemory const & vertexBufferMemory,
	std::vector<T> const& vertices)
{
	assert(!vertexBufferMemory);
	assert(device);

	// copy vertices
	vk::DeviceSize const bufferSize = sizeof(vertices.front()) * vertices.size();

	auto const data = device->mapMemory(vertexBufferMemory.get(), 0, bufferSize);
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	device->unmapMemory(vertexBufferMemory.get());

	//copyBuffer(stagingBuffer.get(), vertexBuffer.get(), bufferSize);

	 
}

template<typename T>
inline void VulkanParticleRenderer::updateVertexBuffer(std::vector<vk::UniqueDeviceMemory> const& vertexBufferMemory, 
	std::vector<T> const& vertices)
{
	assert(!vertexBufferMemory.empty());
	assert(device);

	assert(currentImageResultValue.result == vk::Result::eSuccess);
	uint32_t const imageIndex = currentImageResultValue.value;

	// copy vertices
	vk::DeviceSize const bufferSize = sizeof(vertices.front()) * vertices.size();

	auto & vertexMem = vertexBufferMemory[imageIndex].get();

	auto const data = device->mapMemory(vertexMem, 0, bufferSize);
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	device->unmapMemory(vertexMem);
}


template<typename T, typename TFunc>
inline void VulkanParticleRenderer::updateVertexBuffer(std::vector<vk::UniqueDeviceMemory> const& vertexBufferMemory,
	TFunc & func, size_t const size, bool const fullVertexBufferUpdateRequired)
{
	assert(!vertexBufferMemory.empty());
	assert(device);

	assert(currentImageResultValue.result == vk::Result::eSuccess);
	uint32_t const imageIndex = currentImageResultValue.value;

	auto& vertexMem = vertexBufferMemory[imageIndex].get();

	// copy vertices
	vk::DeviceSize const bufferSize = sizeof(T) * size;

	auto const data = device->mapMemory(vertexMem, 0, bufferSize);

		auto begin = reinterpret_cast<T*>(data);
		auto end = reinterpret_cast<T*>(data) + size;

		func(begin, end,  fullVertexBufferUpdateRequired);

	device->unmapMemory(vertexMem);
}
