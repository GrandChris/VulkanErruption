//
// @file:   dynamic_point_object.h
// @author: GrandChris
// @date:   2021-09-08
// @brief:  Draws points on the screen
//

#pragma once

#include "include_glm.h"
// #include <vulkan/vulkan.hpp>

#include "vulkan_erruption/shader_object/shader_object.h"

#include "vulkan_erruption/components/advanced_vertex_buffer.h"

#include "vulkan_erruption/components/simple_uniform_buffer.h"
#include "vulkan_erruption/components/simple_descriptor_set_layout.h"
#include "vulkan_erruption/components/simple_descriptor_pool.h"
#include "vulkan_erruption/components/simple_descriptor_sets.h"

#include "vulkan_erruption/components/advanced_pipeline.h"
#include "vulkan_erruption/components/advanced_command_buffer.h"

#include "dynamic_point_object_shader.h"

class DynamicPointObject : public ShaderObject
{
public:
    DynamicPointObject(DynamicPointObjectShader const & shader, size_t const size);

    // Inherited Functions
    void setup(RenderEngineInterface & engine) override;
    void draw(RenderEngineInterface & engine, size_t const imageIndex) override;
    void cleanup(RenderEngineInterface & engine) override;

protected:
    virtual void doUpdateVertexBuffer(void * data, size_t const size) = 0;
    virtual void doUpdateUniformBuffer(void * data, size_t const size) = 0;

private:
    using UpdateFunction = void (*)(void * data, size_t size);

    UpdateFunction VertexBufferUpdateFunction = nullptr;

    DynamicPointObjectShader const & mShader;
    size_t const mSize; 

    AdvancedVertexBuffer mVertexBuffer;

    SimpleUniformBuffer mUniformBuffer;
    SimpleDescriptorSetLayout mDescriptorSetLayout;
    SimpleDescriptorPool mDescriptorPool;
    SimpleDescriptorSets mDescriptorSets;

    AdvancedGraphicsPipeline mPipeline;
    AdvancedCommandBuffer mCommands;
};


#include "event/event.h"

template<typename TObject, typename TShader>
class ConcreteShaderObject : public TObject {
public:
    using VertexBufferElement = TShader::VertexBufferElement;
    using UniformBuffer = TShader::UnformBuffer;

    Event<void(std::span<VertexBufferElement>)> updateVertexBuffer;
    Event<void(UniformBuffer&)> updateUniformBuffer;

    ConcreteShaderObject(size_t size) 
        : DynamicPointObject(mShader, size * sizeof(VertexBufferElement))
    {

    }

private:
    void doUpdateVertexBuffer(void * data, size_t const size) override
    {
        VertexBufferElement * begin = reinterpret_cast<VertexBufferElement *>(data);
        VertexBufferElement * end = reinterpret_cast<VertexBufferElement *>(reinterpret_cast<uint8_t *>(data) + size);
        std::span<VertexBufferElement> span(begin, end);
        updateVertexBuffer(span);
    }

    void doUpdateUniformBuffer(void * data, size_t const size) override
    {
        assert(size == sizeof(UniformBuffer));
        UniformBuffer * elem = reinterpret_cast<UniformBuffer *>(data);
        updateUniformBuffer(*elem);
    }

    TShader mShader;
};