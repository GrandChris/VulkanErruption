///////////////////////////////////////////////////////////////////////////////
// File: ParticleRenderer.h
// Date: 13.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Drawing particles with Vulkan
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RenderObject.h"


#include <memory>
#include <thread>
#include <iostream>
#include <thread>
#include <vector>



class ParticleRenderer {
public:
	using uPtr = std::unique_ptr<ParticleRenderer>;

	// D-Tor
	virtual ~ParticleRenderer() = default;

	// Draws 
	//void draw(std::vector<Vertex> const & vertices);
	virtual void run() = 0;
	
	void add(RenderObject::uPtr obj);

	virtual void setVSync(bool const vsyncEnabled) = 0;
	virtual void setWindowSize(size_t const widht, size_t const height, bool const fullscreenEnabled = false) = 0;

	// Gets a Vulkan implementation of this interface
	static uPtr createVulkan();

protected:
	std::vector<RenderObject::uPtr> mObjs;

	bool mFullscreenEnabled = false;
	bool mVSyncEnabled = false;
	size_t mWidth = 800;
	size_t mHeight = 600;

private:
	virtual void create(RenderObject::uPtr const & obj) = 0;
	virtual void draw(RenderObject::uPtr const & obj) = 0;
	virtual void cleanup(RenderObject::uPtr const & obj) = 0;
};



// #######+++++++ Implementation +++++++#######


inline void ParticleRenderer::add(RenderObject::uPtr obj)
{
	create(obj);
	mObjs.push_back(std::move(obj));
}



