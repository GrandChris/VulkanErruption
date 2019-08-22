///////////////////////////////////////////////////////////////////////////////
// File: HelloTriangleApplication.h
// Date: 21.08.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Draws a simple triangle
///////////////////////////////////////////////////////////////////////////////

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#include <vulkan/vulkan.hpp>


#include "upGLFWWindow.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <memory>
#include <optional>



class HelloTriangleApplication{
public:
	void run();

private:
	struct QueueFamilyIndices;

	void initWindow();

	void initVulkan();

		void createInstance();

			std::vector<const char*> getRequiredExtensions();

			bool checkValidationLayerSupport();

		void setupDebugMessenger();

		void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo);

		void pickPhysicalDevice();

			int rateDeviceSuitability(vk::PhysicalDevice const & device);

			bool isDeviceSuitable(vk::PhysicalDevice const & device);

			QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice const& device);

		void createLogicalDevice();

	void mainLoop();

	void cleanup();

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

		bool isComplete() const {
			return graphicsFamily.has_value();
		}
	};

	// Logical device and queues

	vk::UniqueDevice device;

	vk::Queue graphicsQueue;

};




