//
// @file: render_engine_interface.h
// @author: GrandChris
// @date: 26.06.2021
// @brief: Exports the function required to control the render engine
//

#include <vulkan.hpp>

class RenderEngineInterface
{
public:
    virtual void createDesriptorSetLayout(vk::UniqueDescriptorSetLayout & descriptorSetLayout, 
	vk::ShaderStageFlags const shaderStageFlag, bool const useStorageBuffer) = 0;

    virtual void createGraphicsPipeline() = 0;
    virtual void createVertexBuffer() = 0;
    virtual void createStorageBuffers() = 0;
    virtual void createUniformBuffers() = 0;
    virtual void createDescriptorPool() = 0;
    virtual void createDescriptorSets() = 0;
    virtual void recordCommands() = 0;
    virtual void updateUniformBuffer() = 0;
    virtual void updateVertexBuffer() = 0;
};




