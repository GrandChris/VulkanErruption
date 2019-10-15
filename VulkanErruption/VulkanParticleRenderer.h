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
#include "Vertex.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <memory>
#include <optional>


class VulkanParticleRenderer : public ParticleRenderer {
public:
	// Geerbt über ParticleRenderer
	virtual void doDraw(std::vector<Vertex> const & vertices) override;

	void run();

private:
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

		void createGraphicsPipeline();

			vk::UniqueShaderModule createShaderModule(std::vector<char> const & code);

		void createFramebuffers();

		void createCommandPool();

		void createVertexBuffer();
			
			uint32_t findMemoryType(uint32_t const typeFilter, vk::MemoryPropertyFlags const & properties);

			void createBuffer(vk::DeviceSize const size, vk::BufferUsageFlags const usage,
				vk::MemoryPropertyFlags const properties, vk::UniqueBuffer& buffer, vk::UniqueDeviceMemory& bufferMemory);

			void copyBuffer(vk::Buffer const & srcBuffer, vk::Buffer & dstBuffer, vk::DeviceSize size);

		void createUniformBuffers();

			void createDescriptorPool();

			void createDescriptorSets();

		void createCommandBuffers();

		void createSyncObjects();

	void mainLoop();

		void drawFrame();

			void updateUniformBuffer(uint32_t currentImage);

	void cleanup();

	void recreateSwapChain();

	void cleanupSwapChain();

	// Instance

	const int WIDTH = 800;
	const int HEIGHT = 600;

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

	vk::UniquePipelineLayout pipelineLayout;

	// Graphics pipeline

	vk::UniquePipeline graphicsPipeline;

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

	// Swap chain recreation

	bool framebufferResized = false;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	// Vertex input description

	std::vector<glmVertex> vertices =
	{
		{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	// Vertex buffer creation

	vk::UniqueDeviceMemory vertexBufferMemory;

	vk::UniqueBuffer vertexBuffer;

	// Staging buffer

	// Descriptor layout and buffer

	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	vk::UniqueDescriptorSetLayout descriptorSetLayout;
	
	std::vector<vk::UniqueDeviceMemory> uniformBuffersMemory;

	std::vector<vk::UniqueBuffer> uniformBuffers;

	// Descriptor pool and sets
	vk::UniqueDescriptorPool descriptorPool;

	std::vector<vk::DescriptorSet> descriptorSets;


};



