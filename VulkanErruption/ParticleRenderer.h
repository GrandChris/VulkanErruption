///////////////////////////////////////////////////////////////////////////////
// File: ParticleRenderer.h
// Date: 13.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Drawing particles with Vulkan
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RenderObject.h"



#include <vector>
#include <memory>
#include <thread>
#include <iostream>
#include <thread>
#include <vector>

//struct Vertex
//{
//	glm::vec2 pos;
//	glm::vec3 color;
//};



class ParticleRenderer {
public:
	using uPtr = std::unique_ptr<ParticleRenderer>;

	// D-Tor
	virtual ~ParticleRenderer() = default;

	// Draws 
	//void draw(std::vector<Vertex> const & vertices);
	virtual void run() = 0;
	
	void add(RenderObject::uPtr obj);

	// Gets a Vulkan implementation of this interface
	static uPtr createVulkan();

	// Waits for the thread to finish
	void join();

protected:
	std::vector<RenderObject::uPtr> mObjs;

private:
	//virtual void doDraw(std::vector<Vertex> const& vertices) = 0;

	virtual void create(RenderObject::uPtr const & obj) = 0;
	virtual void draw(RenderObject::uPtr const & obj) = 0;
	virtual void cleanup(RenderObject::uPtr const & obj) = 0;

	std::thread thread;
};



// #######+++++++ Implementation +++++++#######

//
//inline void ParticleRenderer::draw(std::vector<Vertex> const& vertices)
//{
//	thread = std::thread([&]()
//		{
//			try
//			{
//				doDraw(vertices);
//				return 0;
//			}
//			catch (std::exception const& e)
//			{
//				std::cerr << std::endl;
//				std::cerr << "exception: " << e.what() << std::endl;
//				std::cerr << std::endl;
//				return 1;
//			}
//		});
//}


inline void ParticleRenderer::add(RenderObject::uPtr obj)
{
	create(obj);
	mObjs.push_back(std::move(obj));
}

inline void ParticleRenderer::join()
{
	thread.join();
}