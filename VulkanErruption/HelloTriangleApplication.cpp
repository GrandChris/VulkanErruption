///////////////////////////////////////////////////////////////////////////////
// File: HelloTriangleApplication.cpp
// Date: 21.08.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Draws a simple triangle
///////////////////////////////////////////////////////////////////////////////


#include "HelloTriangleApplication.h"



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
}

void HelloTriangleApplication::createInstance()
{
	vk::ApplicationInfo appInfo;
	appInfo.setPApplicationName("Hello Triangle");
	appInfo.setApplicationVersion(VK_MAKE_VERSION(1, 0, 0));
	appInfo.setPEngineName("No Engine");
	appInfo.setEngineVersion(VK_MAKE_VERSION(1, 0, 0));
	appInfo.setApiVersion(VK_API_VERSION_1_1);

	vk::InstanceCreateInfo createInfo;
	createInfo.setPApplicationInfo(&appInfo);

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.setEnabledExtensionCount(glfwExtensionCount);
	createInfo.setPpEnabledExtensionNames(glfwExtensions);

	instance = vk::createInstanceUnique(createInfo);
	

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
