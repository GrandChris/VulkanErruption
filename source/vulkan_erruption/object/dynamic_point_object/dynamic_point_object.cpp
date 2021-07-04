


#include "dynamic_point_object.h"


DynamicPointObject_NoTemplate::DynamicPointObject_NoTemplate(DynamicPointShader & shader, 
	size_t vertexBufferElementSize,
	size_t storageBufferElementSize,
	size_t uniformDataSize, 
	size_t vertexBufferSize,
	size_t storageBufferSize)
:	mShader(shader),
	mVertexBufferElementSize(vertexBufferElementSize),
	mStorageBufferElementSize(storageBufferElementSize),
	mUniformDataSize(uniformDataSize),
	mVertexBufferSize(vertexBufferSize),
	mStorageBufferSize(storageBufferSize)
{
	
}


void DynamicPointObject_NoTemplate::setup(RenderEngineInterface& engine) 
{
    createDescriptorSetLayout(engine);
	createGraphicsPipeline(engine);
	createVertexBuffer(engine);
	createStorageBuffer(engine);
	createUniformBuffer(engine);
	createDescriptorPool(engine);
	createDescriptorSets(engine);
	createCommandBuffer(engine);

	mFullBufferUpdateRequired = true;
}

void DynamicPointObject_NoTemplate::draw(RenderEngineInterface& engine) 
{
    drawFrame(engine);
}

void DynamicPointObject_NoTemplate::cleanup(RenderEngineInterface& engine) 
{
    descriptorSets.clear();
	descriptorPool.reset();

	// need not be deleted?
	uniformBuffers.clear();
	uniformBuffersMemory.clear();
	storageBuffers.clear();
	storageBufferMemory.clear();
	vertexBuffers.clear();
	vertexBufferMemory.clear();

	graphicsPipeline.reset();
	pipelineLayout.reset();

	descriptorSetLayout.reset();
}


vk::VertexInputBindingDescription DynamicPointObject_NoTemplate::getVertexBindingDescription()
{
	vk::VertexInputBindingDescription bindingDescription;
	bindingDescription.setBinding(0);
	bindingDescription.setStride(mVertexBufferElementSize);
	bindingDescription.setInputRate(vk::VertexInputRate::eVertex);

	return bindingDescription;
}

// Create functions
void DynamicPointObject_NoTemplate::createDescriptorSetLayout(RenderEngineInterface& engine) 
{
	vk::ShaderStageFlags shaderStageFlags = vk::ShaderStageFlagBits::eVertex;
	if (!mShader.getGeometryShaderCode().empty()) {
		shaderStageFlags =  shaderStageFlags | vk::ShaderStageFlagBits::eGeometry;
	}

	engine.createDescriptorSetLayout(descriptorSetLayout, shaderStageFlags, mStorageBufferElementSize > 0);
}

void DynamicPointObject_NoTemplate::createGraphicsPipeline(RenderEngineInterface& engine) 
{

	// vertex shader specialization info
	vk::SpecializationInfo specializationInfoVertex = {};
	auto const specializationInfoVertexData = mShader.getSpecializationInfoVertexShader();
	if (!specializationInfoVertexData.entries.empty())
	{
		specializationInfoVertex.setMapEntryCount(static_cast<uint32_t>(specializationInfoVertexData.entries.size()));
		specializationInfoVertex.setPMapEntries(specializationInfoVertexData.entries.data());
		specializationInfoVertex.setDataSize(specializationInfoVertexData.data.size());
		specializationInfoVertex.setPData(specializationInfoVertexData.data.data());
	}

	vk::SpecializationInfo specializationInfoGeometry = {};
	auto const specializationInfoGeometryData = mShader.getSpecializationInfoGeometryShader();
	if (!specializationInfoGeometryData.entries.empty())
	{
		specializationInfoGeometry.setMapEntryCount(static_cast<uint32_t>(specializationInfoGeometryData.entries.size()));
		specializationInfoGeometry.setPMapEntries(specializationInfoGeometryData.entries.data());
		specializationInfoGeometry.setDataSize(specializationInfoGeometryData.data.size());
		specializationInfoGeometry.setPData(specializationInfoGeometryData.data.data());
	}

	vk::SpecializationInfo specializationInfoFragment = {};
	auto const specializationInfoFragmentData = mShader.getSpecializationInfoFragmentShader();
	if (!specializationInfoFragmentData.entries.empty())
	{
		specializationInfoFragment.setMapEntryCount(static_cast<uint32_t>(specializationInfoFragmentData.entries.size()));
		specializationInfoFragment.setPMapEntries(specializationInfoFragmentData.entries.data());
		specializationInfoFragment.setDataSize(specializationInfoFragmentData.data.size());
		specializationInfoFragment.setPData(specializationInfoFragmentData.data.data());
	}

	engine.createGraphicsPipeline(pipelineLayout, graphicsPipeline, 
		mShader.getVertexShaderCode(), mShader.getGeometryShaderCode(), mShader.getFragmentShaderCode(),
		descriptorSetLayout, getVertexBindingDescription(), mShader.getVertexAttributeDescriptions(), 
		specializationInfoVertex, specializationInfoGeometry, specializationInfoFragment);
}

void DynamicPointObject_NoTemplate::createVertexBuffer(RenderEngineInterface& engine) 
{
	if (mVertexBufferSize > 0)
	{
		size_t const bufferSize = mVertexBufferElementSize * mVertexBufferSize;

		engine.createVertexBuffers(vertexBufferMemory, vertexBuffers, bufferSize);
	}
}

void DynamicPointObject_NoTemplate::createStorageBuffer(RenderEngineInterface& engine) 
{
	if (mStorageBufferSize > 0)
	{
		size_t const bufferSize = mStorageBufferElementSize * mStorageBufferSize;

		engine.createStorageBuffers(storageBufferMemory, storageBuffers, bufferSize);
	}
}

void DynamicPointObject_NoTemplate::createUniformBuffer(RenderEngineInterface& engine) 
{
	engine.createUniformBuffers(uniformBuffersMemory, uniformBuffers, mUniformDataSize);
}

void DynamicPointObject_NoTemplate::createDescriptorPool(RenderEngineInterface& engine) 
{
	engine.createDescriptorPool(descriptorPool);
}

void DynamicPointObject_NoTemplate::createDescriptorSets(RenderEngineInterface& engine) 
{
	size_t const StorageBufferSize = mStorageBufferElementSize * mStorageBufferSize;

	engine.createDescriptorSets(descriptorPool, descriptorSets, descriptorSetLayout, 
		uniformBuffers, mUniformDataSize,
		storageBuffers, StorageBufferSize);
}

void DynamicPointObject_NoTemplate::createCommandBuffer(RenderEngineInterface& engine) 
{
	engine.recordCommands(pipelineLayout, 
		graphicsPipeline,
		vertexBuffers, 
		descriptorSets, 
		mVertexBufferSize,
		"DynamicPointObject");
}

// Draw functions
void DynamicPointObject_NoTemplate::drawFrame(RenderEngineInterface& engine)
{

}
