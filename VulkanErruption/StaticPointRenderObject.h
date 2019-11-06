///////////////////////////////////////////////////////////////////////////////
// File: StaticPointRenderObject.h
// Date: 20.10.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: A object, that can be drown through a vulkan particle renderer
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RenderObject.h"

#include "extern/glm-0.9.9.5/glm/glm/glm.hpp"

#include <vector>
#include <memory>



class StaticPointRenderObject : public RenderObject
{
public:
	using uPtr = std::unique_ptr<StaticPointRenderObject>;

	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;
	};

	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	void setVertices(std::vector<Vertex> const& vertices);
	void setPosition(glm::vec3 const& pos);


	static uPtr createVulkan();

protected:
	std::vector<Vertex> mVertices;

	glm::vec3 mPos;
};

// #######+++++++ Implementation +++++++#######

inline void StaticPointRenderObject::setVertices(std::vector<Vertex> const& vertices)
{
	mVertices = vertices;
}

inline void StaticPointRenderObject::setPosition(glm::vec3 const& pos)
{
	mPos = pos;
}
