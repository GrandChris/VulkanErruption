///////////////////////////////////////////////////////////////////////////////
// File: ParticleRenderer.h
// Date: 13.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Drawing particles with Vulkan
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>
#include <thread>
#include <iostream>
#include <thread>

struct Vertex
{
	struct pos
	{
		float x;
		float y;
	}pos;

	struct color
	{
		float r;
		float g;
		float b;
	}color;
};



class ParticleRenderer {
public:
	using uPtr = std::unique_ptr<ParticleRenderer>;

	// D-Tor
	virtual ~ParticleRenderer() = default;

	// Draws 
	void draw(std::vector<Vertex> const & vertices);

	// Gets a Vulkan implementation of this interface
	static uPtr createVulkan();

	// Waits for the thread to finish
	void join();

private:
	virtual void doDraw(std::vector<Vertex> const& vertices) = 0;

	std::thread thread;
};



// #######+++++++ Implementation +++++++#######


inline void ParticleRenderer::draw(std::vector<Vertex> const& vertices)
{
	thread = std::thread([&]()
		{
			try
			{
				doDraw(vertices);
				return 0;
			}
			catch (std::exception const& e)
			{
				std::cerr << std::endl;
				std::cerr << "exception: " << e.what() << std::endl;
				std::cerr << std::endl;
				return 1;
			}
		});
}

inline void ParticleRenderer::join()
{
	thread.join();
}