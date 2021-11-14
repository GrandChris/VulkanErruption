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
    // vertex buffer
    mVertexBuffer.createVertexBuffer(engine, mSize * mShader.getVertexElementSize());
    
    // uniform buffer
    mUniformBuffer.createUniformBuffer(engine, mShader.getUniformBufferSize());
    mDescriptorSetLayout.createDescriptorSetLayout(engine, mShader.getUniformBindingDescription());
    mDescriptorPool.createDescriptorPool(engine);
    mDescriptorSets.createDescriptorSets(engine,
        mDescriptorPool.getDescriptorPool(),
        mDescriptorSetLayout.getDescriptorSetLayout(),
        mUniformBuffer.getBuffers(),
        mShader.getUniformBufferSize());

    // pipeline
    mPipeline.createGraphicsPipeline(engine, 
        mShader.getVertexShaderCode(),
        mShader.getGeometryShaderCode(),
        mShader.getFragmentShaderCode(),
        mShader.getVertexBindingDescription(),
        mShader.getVertexAttributeDescriptions(),
        mDescriptorSetLayout.getDescriptorSetLayout(),
        mShader.getInputTopology(),
        mShader.getVertexShaderSpecializationMap(),
        mShader.getGeometryShaderSpecializationMap(),
        mShader.getFragmentShaderSpecializationMap(),
        mShader.getVertexShaderSpecializationData(),
        mShader.getGeometryShaderSpecializationData(),
        mShader.getFragmentShaderSpecializationData()
        );

    // commands
    mCommands.recordCommands(engine,
        mPipeline.getPipelineLayout(),
        mPipeline.getPipeline(),
        mVertexBuffer.getBuffers(),
        mDescriptorSets.getDescriptorSets(),
        mSize);
}

void DynamicPointObject::draw(RenderEngineInterface & engine, size_t const imageIndex)
{
    bool updateRequired = false;
    if(mUpdateRequiredCounter > 0) {
        updateRequired = true;
        --mUpdateRequiredCounter;
    }


    Delegate<void(void * const, size_t, bool)> funcVertex;
    funcVertex.set<DynamicPointObject::doUpdateVertexBuffer>(*this);

    mVertexBuffer.update(engine, imageIndex, updateRequired, funcVertex);

    Delegate<void(void * const, size_t)> funcUniform;
    funcUniform.set<DynamicPointObject::doUpdateUniformBuffer>(*this);

    mUniformBuffer.update(engine, imageIndex, funcUniform);
}

void DynamicPointObject::cleanup(RenderEngineInterface & engine)
{
    mCommands.clear();
    
    mPipeline.clear();

    mDescriptorSets.clear();
    mDescriptorPool.clear();
    mDescriptorSetLayout.clear();
    mUniformBuffer.clear();

    mVertexBuffer.clear();

    mUpdateRequiredCounter = 3;
}