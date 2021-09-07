//
// @file:   hello_triangle.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  Most simple possible object
//

#pragma once

#include "include_glm.h"
// #include <vulkan/vulkan.hpp>

#include "vulkan_erruption/shader_object/shader_object.h"

#include "vulkan_erruption/components/simple_vertex_buffer.h"
#include "vulkan_erruption/components/simple_pipeline.h"
#include "vulkan_erruption/components/simple_command_buffer.h"

#include "hello_triangle_shader.h"

class HelloTriangle : public ShaderObject
{
public:
    HelloTriangle(HelloTriangleShader & shader);

    void setup(RenderEngineInterface & engine) override;
    void draw(RenderEngineInterface & engine, size_t const imageIndex) override;
    void cleanup(RenderEngineInterface & engine) override;

private:
    std::vector<glm::vec3> vertexBufferData = {
        {0.5f, 0.5f, 0.0f},
        {0.5f, 0.0f, 0.0f},
        {0.0f, 0.5f, 0.0f}
    };

    size_t const mVertexElementSize = sizeof(glm::vec3);
    size_t const mVertexDataSize = std::size(vertexBufferData);
    size_t const mVertexBufferSize = mVertexElementSize * mVertexDataSize;

    HelloTriangleShader & mShader;

    SimpleVertexBuffer mVertexBuffer;
    SimpleGraphicsPipeline mPipeline;
    SimpleCommandBuffer mCommands;
};