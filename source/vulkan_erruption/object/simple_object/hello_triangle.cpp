//
// @file:   hello_triangle_shader.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  Shader for hello triangle
//

#include "hello_triangle.h"


HelloTriangle::HelloTriangle(HelloTriangleShader & shader)
    : mShader(shader)
{

}


void HelloTriangle::setup(RenderEngineInterface & engine)
{
    assert(mVertexElementSize == mShader.getVertexBufferElementSize());

    mVertexBuffer.createVertexBuffer(engine, mVertexBufferSize);

    mPipeline.createGraphicsPipeline(engine, 
        mShader.getVertexShaderCode(),
        mShader.getFragmentShaderCode(),
        mShader.getVertexBindingDescription(),
        mShader.getVertexAttributeDescriptions());

    mCommands.recordCommands(engine,
        mPipeline.getPipelineLayout(),
        mPipeline.getPipeline(),
        mVertexBuffer.getBuffers(),
        mVertexBufferSize);
}

void HelloTriangle::draw(RenderEngineInterface & engine, size_t const imageIndex)
{
    mVertexBuffer.update(engine, imageIndex, vertexBufferData);
}

void HelloTriangle::cleanup(RenderEngineInterface & engine)
{
    mCommands.clear();
    mPipeline.clear();
    mVertexBuffer.clear();
}