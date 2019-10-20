///////////////////////////////////////////////////////////////////////////////
// File: RenderObject.h
// Date: 20.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: A object, that can be drown through a particle renderer
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

class VulkanParticleRenderer;

class RenderObject
{
public:
	using uPtr = std::unique_ptr<RenderObject>;

	// D-Tor
	virtual ~RenderObject() = default;

	virtual void create(VulkanParticleRenderer& engine) = 0;
	virtual void draw(VulkanParticleRenderer& engine) = 0;
	virtual void cleanup(VulkanParticleRenderer& engine) = 0;
};