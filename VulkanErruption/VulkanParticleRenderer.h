///////////////////////////////////////////////////////////////////////////////
// File: VulkanParticleRenderer.h
// Date: 13.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Drawing particles with Vulkan
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ParticleRenderer.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>

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
	VulkanParticleRenderer();

	// D-Tor
	~VulkanParticleRenderer();

	// Geerbt über ParticleRenderer
	//virtual void doDraw(std::vector<Vertex> const & vertices) override;

	// Geerbt über ParticleRenderer
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

		void createLogicalDevice();

		bool checkDeviceExtensionSupport(vk::PhysicalDevice const& device);

		void createSwapChain();

			SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice const & device);

			vk::SurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const & availableFormats);

			vk::PresentModeKHR chooseSwapPresentMode(std::vector<vk::PresentModeKHR> const & availablePresentModes);

			vk::Extent2D chooseSwapExtent(vk::SurfaceCapabilitiesKHR const& capabilities);

		void createImageViews();

		void createRenderPass();

		void createDescriptorSetLayout();

		void createGraphicsPipeline(vk::UniquePipelineLayout & pipelineLayout, 
			vk::UniquePipeline & graphicsPipeline,
			std::vector<char> const & vertShaderCode, std::vector<char> const & fragShaderCode,
			vk::VertexInputBindingDescription const & bindingDescription,
			std::vector<vk::VertexInputAttributeDescription>const & attributeDescriptions);
		//void createGraphicsPipeline();

			vk::UniqueShaderModule createShaderModule(std::vector<char> const & code);

		void createFramebuffers();

		void createCommandPool();

		void createDepthResources();

			vk::Format findDepthFormat();

				vk::Format findSupportedFormat(std::vector<vk::Format> const& candidates,
					vk::ImageTiling tiling, vk::FormatFeatureFlags features);

			bool hasStencilComponent(vk::Format format);

			void createImage(uint32_t const width, uint32_t const height, vk::Format const & format,
				vk::ImageTiling const & tiling, vk::ImageUsageFlags const & usage, 
				vk::MemoryPropertyFlags const & properties,
				vk::UniqueImage& image, vk::UniqueDeviceMemory & imageMemory);

			vk::UniqueImageView createImageView(vk::Image const& image, vk::Format const& format,
				vk::ImageAspectFlags const& aspectFlags);

		template<typename T>
		void createVertexBuffer(vk::UniqueDeviceMemory & vertexBufferMemory, vk::UniqueBuffer & vertexBuffer,
			std::vector<T> const& vertices);
		//void createVertexBuffer();
			
			uint32_t findMemoryType(uint32_t const typeFilter, vk::MemoryPropertyFlags const & properties);

			void createBuffer(vk::DeviceSize const size, vk::BufferUsageFlags const usage,
				vk::MemoryPropertyFlags const properties, vk::UniqueBuffer& buffer, vk::UniqueDeviceMemory& bufferMemory);

			void copyBuffer(vk::Buffer const & srcBuffer, vk::Buffer & dstBuffer, vk::DeviceSize size);

		void createUniformBuffers(std::vector<vk::UniqueDeviceMemory>& uniformBuffersMemory,
				std::vector<vk::UniqueBuffer>& uniformBuffers, size_t const UniformBufferObjectSize);
		//void createUniformBuffers();

			void createDescriptorPool();

			void createDescriptorSets(std::vector<vk::DescriptorSet> & descriptorSets,
				std::vector<vk::UniqueBuffer> const & uniformBuffers,
				size_t const UniformBufferObjectSize);
			//void createDescriptorSets();

		void createCommandBuffers(std::vector<vk::UniqueCommandBuffer> & commandBuffers,
			vk::UniquePipelineLayout const & pipelineLayout,
			vk::UniquePipeline const & graphicsPipeline,
			vk::UniqueBuffer const & vertexBuffer,
			std::vector<vk::DescriptorSet> const & descriptorSets,
			size_t const verticesCount);
		//void createCommandBuffers();

		void createSyncObjects();

	void mainLoop();

		template<typename T>
		void drawFrame(std::vector<vk::UniqueCommandBuffer> const& commandBuffers,
			std::vector<vk::UniqueDeviceMemory> const & uniformBuffersMemory, 
			T const & uniformBufferObject);
		//void drawFrame();

			template<typename T>
			void updateUniformBuffer(uint32_t currentImage,
				std::vector<vk::UniqueDeviceMemory> const& uniformBuffersMemory,
				T const& uniformBufferObject);
			//void updateUniformBuffer(uint32_t currentImage);

	void cleanup();

	void recreateSwapChain();

	void cleanupSwapChain();

	// Instance

	int WIDTH = 800;
	int HEIGHT = 600;

	upGLFWWindow window;

	vk::UniqueInstance instance;

	// Validation layers

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

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
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
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

	//std::vector<vk::UniqueCommandBuffer> commandBuffers;

	// Rendering and presentation

	int const MAX_FRAMES_IN_FLIGHT = 2;

	std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
	std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
	std::vector<vk::UniqueFence> inFlightFences;

	size_t currentFrame = 0;

	// Swap chain recreation

	bool framebufferResized = false;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

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

	vk::UniqueDescriptorSetLayout descriptorSetLayout;
	
	//std::vector<vk::UniqueDeviceMemory> uniformBuffersMemory;

	//std::vector<vk::UniqueBuffer> uniformBuffers;

	// Descriptor pool and sets
	vk::UniqueDescriptorPool descriptorPool;

	//std::vector<vk::DescriptorSet> descriptorSets;


	// Depth buffering
	vk::UniqueDeviceMemory depthImageMemory;
	vk::UniqueImage depthImage;
	vk::UniqueImageView depthImageView;



	// Show FPS
	glfwFPS mFPS;





};


// #######+++++++ Implementation +++++++#######

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

template<typename T>
inline void VulkanParticleRenderer::createVertexBuffer(vk::UniqueDeviceMemory& vertexBufferMemory, vk::UniqueBuffer& vertexBuffer, std::vector<T> const& vertices)
{
	assert(device);
	
	vk::DeviceSize const bufferSize = sizeof(vertices.front()) * vertices.size();
	
	vk::UniqueDeviceMemory stagingBufferMemory;
	vk::UniqueBuffer stagingBuffer;
	
	createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		stagingBuffer, stagingBufferMemory
	);
	
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
inline void VulkanParticleRenderer::drawFrame(std::vector<vk::UniqueCommandBuffer> const& commandBuffers, 
	std::vector<vk::UniqueDeviceMemory> const& uniformBuffersMemory, 
	T const& uniformBufferObject)
{
	device->waitForFences(inFlightFences[currentFrame].get(), VK_TRUE, UINT64_MAX);

	vk::ResultValue<uint32_t> res(vk::Result::eSuccess, 0);
	try {
		res = device->acquireNextImageKHR(swapChain.get(), UINT64_MAX, imageAvailableSemaphores[currentFrame].get(), nullptr);
		if (res.result == vk::Result::eErrorOutOfDateKHR)	// window was resized, swap chain is now incompatible
		{
			recreateSwapChain();
			return;
		}
		else if (res.result != vk::Result::eSuccess && res.result != vk::Result::eSuboptimalKHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}
	}
	catch (vk::OutOfDateKHRError const&)
	{
		recreateSwapChain();
		return;
	}

	uint32_t const imageIndex = res.value;

	updateUniformBuffer(imageIndex, uniformBuffersMemory, uniformBufferObject);

	vk::SubmitInfo submitInfo;

	vk::Semaphore waitSemaphore[] = { imageAvailableSemaphores[currentFrame].get() };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.setWaitSemaphoreCount(1);
	submitInfo.setPWaitSemaphores(waitSemaphore);
	submitInfo.setPWaitDstStageMask(waitStages);
	submitInfo.setCommandBufferCount(1);
	submitInfo.setPCommandBuffers(&commandBuffers[imageIndex].get());

	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame].get() };
	submitInfo.setSignalSemaphoreCount(1);
	submitInfo.setPSignalSemaphores(signalSemaphores);

	device->resetFences(inFlightFences[currentFrame].get());

	graphicsQueue.submit(submitInfo, inFlightFences[currentFrame].get());

	vk::PresentInfoKHR presentInfo;
	presentInfo.setWaitSemaphoreCount(1);
	presentInfo.setPWaitSemaphores(signalSemaphores);
	vk::SwapchainKHR swapChains[] = { swapChain.get() };
	presentInfo.setSwapchainCount(1);
	presentInfo.setPSwapchains(swapChains);
	presentInfo.setPImageIndices(&imageIndex);
	presentInfo.setPResults(nullptr); // optional

	try {
		auto const resPresent = presentQueue.presentKHR(presentInfo);
		if (resPresent == vk::Result::eErrorOutOfDateKHR || resPresent == vk::Result::eSuboptimalKHR || framebufferResized)	// window was resized, swap chain is now incompatible
		{
			framebufferResized = false;
			recreateSwapChain();
		}
		else if (res.result != vk::Result::eSuccess) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}
	}
	catch (vk::OutOfDateKHRError const&)
	{
		framebufferResized = false;
		recreateSwapChain();
	}


	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

template<typename T>
inline void VulkanParticleRenderer::updateUniformBuffer(uint32_t currentImage, 
	std::vector<vk::UniqueDeviceMemory> const& uniformBuffersMemory, 
	T const& uniformBufferObject)
{
	assert(!uniformBuffersMemory.empty());
	assert(device);

	static auto const startTime = std::chrono::high_resolution_clock::now();

	auto const currentTime = std::chrono::high_resolution_clock::now();

	float const time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	T ubo = uniformBufferObject;
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1; // invert Y for Vulkan

	auto const data = device->mapMemory(uniformBuffersMemory[currentImage].get(), 0, sizeof(ubo));
	memcpy(data, &ubo, static_cast<size_t>(sizeof(ubo)));
	device->unmapMemory(uniformBuffersMemory[currentImage].get());
}
