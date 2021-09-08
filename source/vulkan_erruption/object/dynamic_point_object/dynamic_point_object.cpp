//
// @file:   dynamic_point_object.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  Shader for hello triangle
//

#include "dynamic_point_object.h"


DynamicPointObject::DynamicPointObject(DynamicPointObjectShader const & shader, size_t const size)
    : mShader(shader),
      mSize(size)
{

}


void DynamicPointObject::setup(RenderEngineInterface & engine)
{
    mVertexBuffer.createVertexBuffer(engine, mSize);

    mPipeline.createGraphicsPipeline(engine, 
        mShader.getVertexShaderCode(),
        mShader.getFragmentShaderCode(),
        mShader.getVertexBindingDescription(),
        mShader.getVertexAttributeDescriptions());

    mCommands.recordCommands(engine,
        mPipeline.getPipelineLayout(),
        mPipeline.getPipeline(),
        mVertexBuffer.getBuffers(),
        mSize);
}

void DynamicPointObject::draw(RenderEngineInterface & engine, size_t const imageIndex)
{
    Delegate<void(void * const, size_t)> func;
    func.set<DynamicPointObject::doUpdateVertexBuffer>(*this);

    mVertexBuffer.update(engine, imageIndex, func);
}

void DynamicPointObject::cleanup(RenderEngineInterface & engine)
{
    mCommands.clear();
    mPipeline.clear();
    mVertexBuffer.clear();
}