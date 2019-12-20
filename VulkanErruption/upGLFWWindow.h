///////////////////////////////////////////////////////////////////////////////
// File: upGLFWWindow.h
// Date: 21.08.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Unique pointer for GLFWWindow (with included destructor)
///////////////////////////////////////////////////////////////////////////////

#pragma once


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <memory>


class GLFWWindow_deleter
{
public:
	/// <summary>
	/// Deletes memory on the gpu
	/// </summary>
	/// <param name="p">Pointer to allocated memory</param>
	void operator()(GLFWwindow* p) noexcept;
};


using upGLFWWindow = std::unique_ptr<GLFWwindow, GLFWWindow_deleter>;



// #######+++++++ Implementation +++++++#######

inline void GLFWWindow_deleter::operator()(GLFWwindow * window) noexcept
{
	glfwDestroyWindow(window);

	glfwTerminate();
}
