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



class HelloTriangleApplication{
public:
	void run();

private:

	void initWindow();

	void initVulkan();

	void createInstance();

	void mainLoop();

	void cleanup();

	const int WIDTH = 800;
	const int HEIGHT = 600;

	upGLFWWindow window;

	vk::UniqueInstance instance;
};




