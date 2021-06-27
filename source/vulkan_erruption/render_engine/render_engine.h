//
// @file: RenderEngine.h
// @author: GrandChris
// @date: 26.06.2021
// @brief: Interface for the render engine
//

// #pragma once

// #include "vulkan_erruption/ShaderObject.h"
// #include <concepts>

#include "event/event.h"
#include "vulkan_erruption/render_engine/key.h"

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
#endif
#include <glm/glm.hpp>
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#include <cstdint>


// Vulkan Coordinate System
//  z
//	 /|    
//   |  __ y
//   |   /| 
//   |  /
//   | /
//   |_____________> x
//
//



class ShaderObject;

class RenderEngine
{
public:
    Event<void(Key)> keyPressed;
	Event<void(Key)> keyReleased;
	Event<void(void)> startOfNextFrame;

    virtual void add(ShaderObject&) = 0;
    virtual void setViewPosition(glm::vec3 const& eye, glm::vec3 const& view = glm::vec3());
};
