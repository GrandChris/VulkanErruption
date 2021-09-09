//
// @file:   default_vertex_buffer.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  A default unifor buffer descriptor
//

#pragma once

#include "vulkan_erruption/shader_object/render_engine_interface.h"

#include <vulkan/vulkan.hpp>
#include <cassert>

class AdvancedCommandBuffer
{
public:
	void recordCommands(RenderEngineInterface& engine, 
		vk::PipelineLayout const& pipelineLayout, 
        vk::Pipeline const& graphicsPipeline,
        std::vector<vk::UniqueBuffer> const& vertexBuffers,
        std::vector<vk::DescriptorSet> const& descriptorSets,
        size_t const verticesCount);
        // char const * name)

	void clear() {}

private:

};


inline void AdvancedCommandBuffer::recordCommands(RenderEngineInterface& engine, 
		vk::PipelineLayout const& pipelineLayout, 
        vk::Pipeline const& graphicsPipeline,
        std::vector<vk::UniqueBuffer> const& vertexBuffers,
        std::vector<vk::DescriptorSet> const& descriptorSets,
        size_t const verticesCount)
        // char const * name)
{
	assert(pipelineLayout);
	assert(graphicsPipeline);
	assert(!vertexBuffers.empty());
	// assert(!descriptorSets.empty());
	// assert(device);
	// assert(commandPool);
	// assert(!swapChainFramebuffers.empty());

	auto & commandBuffers = engine.getCommandBuffers();


	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		// static size_t iMarker = 0;
		// vk::DebugUtilsLabelEXT debugMarkerInfo = {};
		// debugMarkerInfo.setPLabelName(name);

		// vkCmdBeginDebugUtilsLabelEXT(instance.get(), commandBuffers[i].get(), debugMarkerInfo);

		commandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

		commandBuffers[i]->bindVertexBuffers(0, vertexBuffers[i].get(), vk::DeviceSize());

		commandBuffers[i]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, descriptorSets[i], nullptr);

		commandBuffers[i]->draw(static_cast<uint32_t>(verticesCount), 1, 0, 0);

		// vkCmdEndDebugUtilsLabelEXT(instance.get(), commandBuffers[i].get());
	}
}

