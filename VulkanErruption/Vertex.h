///////////////////////////////////////////////////////////////////////////////
// File: Vertex.h
// Date: 26.08.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Descritpion of an individual vertex
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include <array>


class glmVertex
{
public:
	glm::vec2 pos;
	glm::vec3 color;

	static vk::VertexInputBindingDescription getBindingDescription();

	static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions();

};


// #######+++++++ Implementation +++++++#######

inline vk::VertexInputBindingDescription glmVertex::getBindingDescription()
{
	vk::VertexInputBindingDescription bindingDescription;
	bindingDescription.setBinding(0);
	bindingDescription.setStride(sizeof(glmVertex));
	bindingDescription.setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescription;
}

inline std::array<vk::VertexInputAttributeDescription, 2> glmVertex::getAttributeDescriptions()
{
	std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions;
	attributeDescriptions[0].setBinding(0);
	attributeDescriptions[0].setLocation(0);
	attributeDescriptions[0].setFormat(vk::Format::eR32G32Sfloat);
	attributeDescriptions[0].setOffset(offsetof(glmVertex, pos));
	attributeDescriptions[1].setBinding(0);
	attributeDescriptions[1].setLocation(1);
	attributeDescriptions[1].setFormat(vk::Format::eR32G32B32Sfloat);
	attributeDescriptions[1].setOffset(offsetof(glmVertex, color));


	return attributeDescriptions;
}
