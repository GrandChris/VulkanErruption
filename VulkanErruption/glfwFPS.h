///////////////////////////////////////////////////////////////////////////////
// File: glfwFPS.h
// Date: 15.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Show FPS on the tile of the window
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <cassert>

class glfwFPS {
public:
	void showFPS(GLFWwindow* pWindow);

private:
	double lastTime = 0.0;
	size_t nbFrames = 0;
};


// #######+++++++ Implementation +++++++#######


inline void glfwFPS::showFPS(GLFWwindow* pWindow)
{
	assert(pWindow != nullptr);

	// Measure speed
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	nbFrames++;
	if (delta >= 1.0) { // If last cout was more than 1 sec ago

		size_t const fps = static_cast<size_t>(static_cast<double>(nbFrames) / delta);

		glfwSetWindowTitle(pWindow, std::to_string(fps).c_str());

		nbFrames = 0;
		lastTime = currentTime;
	}
}