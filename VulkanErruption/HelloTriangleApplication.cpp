///////////////////////////////////////////////////////////////////////////////
// File: HelloTriangleApplication.cpp
// Date: 21.08.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Draws a simple triangle
///////////////////////////////////////////////////////////////////////////////


#include "HelloTriangleApplication.h"

// runtime loaded vulkan functions

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
	VkInstance                                  instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
	VkInstance                                  instance,
	VkDebugUtilsMessengerEXT                    messenger,
	const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, messenger, pAllocator);
	}
}

// <<---



void HelloTriangleApplication::run() 
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void HelloTriangleApplication::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = upGLFWWindow(glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr));
}

void HelloTriangleApplication::initVulkan()
{
	createInstance();
	setupDebugMessenger();
}

void HelloTriangleApplication::createInstance()
{
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	vk::ApplicationInfo appInfo;
	appInfo.setPApplicationName("Hello Triangle");
	appInfo.setApplicationVersion(VK_MAKE_VERSION(1, 0, 0));
	appInfo.setPEngineName("No Engine");
	appInfo.setEngineVersion(VK_MAKE_VERSION(1, 0, 0));
	appInfo.setApiVersion(VK_API_VERSION_1_1);

	vk::InstanceCreateInfo createInfo;
	createInfo.setPApplicationInfo(&appInfo);

	vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers)
	{
		createInfo.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()));
		createInfo.setPpEnabledLayerNames(validationLayers.data());

		populateDebugMessengerCreateInfo(debugCreateInfo);
		debugCreateInfo.setMessageSeverity(
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
		);
		createInfo.setPNext(&debugCreateInfo);
	}

	auto const extension = getRequiredExtensions();
	createInfo.setEnabledExtensionCount(static_cast<uint32_t>(extension.size()));
	createInfo.setPpEnabledExtensionNames(extension.data());

	instance = vk::createInstanceUnique(createInfo);
}

std::vector<const char*> HelloTriangleApplication::getRequiredExtensions()
{
	// add extensions for glfw
	uint32_t glfwExtensionCount = 0;
	auto const glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	// add validation layer extension
	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool HelloTriangleApplication::checkValidationLayerSupport()
{
	auto const availableLayers = vk::enumerateInstanceLayerProperties();

	for (char const* layerName : validationLayers)
	{
		bool layerFound = false;

		for (auto const& layerPropberties : availableLayers)
		{
			if (strcmp(layerName, layerPropberties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

void HelloTriangleApplication::setupDebugMessenger()
{
	if (!enableValidationLayers)
	{
		return;
	}

	vk::DebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);
	
	debugMessanger = instance->createDebugUtilsMessengerEXTUnique(createInfo);
}

void HelloTriangleApplication::populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo.setMessageSeverity(
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
	);
	createInfo.setMessageType(
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
	);
	createInfo.setPfnUserCallback(debugCallback);
}

void HelloTriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(window.get())) {
		glfwPollEvents();
	}
}

void HelloTriangleApplication::cleanup()
{

}

VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApplication::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		// Message is important enough to show
	}

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}
