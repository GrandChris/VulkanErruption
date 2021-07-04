


#include "vulkan_erruption/shader_object/shader_object.h"
#include "vulkan_erruption/shader_object/render_engine_interface.h"
#include "dynamic_point_shader.h"
#include "event/event.h"
#include <span>

class DynamicPointObject_NoTemplate 
{
public:
	DynamicPointObject_NoTemplate(DynamicPointShader & shader, 
		size_t vertexBufferElementSize,
		size_t storageBufferElementSize,
		size_t uniformDataSize,
		size_t vertexBufferSize,
		size_t storageBufferSize = 0);

	void setup(RenderEngineInterface& engine);
    void draw(RenderEngineInterface& engine);
    void cleanup(RenderEngineInterface& engine);

public:
	DynamicPointShader const & mShader;
	size_t const mVertexBufferElementSize;
	size_t const mStorageBufferElementSize;
	size_t const mUniformDataSize;

	size_t const mVertexBufferSize;
	size_t const mStorageBufferSize;

	vk::VertexInputBindingDescription getVertexBindingDescription();

	// Create functions
	void createDescriptorSetLayout(RenderEngineInterface& engine);
    void createGraphicsPipeline(RenderEngineInterface& engine);
	void createVertexBuffer(RenderEngineInterface& engine);
    void createStorageBuffer(RenderEngineInterface& engine);
	void createUniformBuffer(RenderEngineInterface& engine);
	void createDescriptorPool(RenderEngineInterface& engine);
	void createDescriptorSets(RenderEngineInterface& engine);
	void createCommandBuffer(RenderEngineInterface& engine);

    // Draw functions
	void drawFrame(RenderEngineInterface& engine);

	// Uniform Buffer description
	vk::UniqueDescriptorSetLayout descriptorSetLayout;

	// Graphics pipeline
	vk::UniquePipelineLayout pipelineLayout;
	vk::UniquePipeline graphicsPipeline;

	// Vertex Buffer
	std::vector<vk::UniqueDeviceMemory> vertexBufferMemory;
	std::vector<vk::UniqueBuffer> vertexBuffers;

	// Storage Buffer
	std::vector<vk::UniqueDeviceMemory> storageBufferMemory;
	std::vector<vk::UniqueBuffer> storageBuffers;

	// Uniform Buffers
	std::vector<vk::UniqueDeviceMemory> uniformBuffersMemory;
	std::vector<vk::UniqueBuffer> uniformBuffers;

	// Descriptor Pool
	vk::UniqueDescriptorPool descriptorPool;

	// Descriptor sets
	std::vector<vk::DescriptorSet> descriptorSets;

    bool mFullBufferUpdateRequired = false;	
};


template<typename TShader>
class DynamicPointObject : public ShaderObject
{
public:
	using VertexBufferElement = TShader::VertexBufferElement;
	using StorageBufferElement = TShader::StorageBufferElement;
	using UniformBufferData = TShader::UniformBufferData;

	Event<void(std::span<VertexBufferElement>&, bool)> updateVertexBuffer;
	Event<void(std::span<StorageBufferElement>&, bool)> updateStorageBuffer;
	Event<void(UniformBufferData &)> updateUniformBuffer;

    DynamicPointObject(TShader & shader, size_t vertexBufferSize);

private:
    
	void setup(RenderEngineInterface& engine) override;
    void draw(RenderEngineInterface& engine) override;
    void cleanup(RenderEngineInterface& engine) override;	

    DynamicPointObject_NoTemplate more;
};

template<typename TShader>
inline DynamicPointObject<TShader>::DynamicPointObject(TShader & shader, size_t vertexBufferSize)
	: more(shader, sizeof(VertexBufferElement), sizeof(StorageBufferElement), sizeof(UniformBufferData), vertexBufferSize)
{

}

template<typename TShader>
inline void DynamicPointObject<TShader>::setup(RenderEngineInterface& engine)
{
	more.setup(engine);
}

template<typename TShader>
inline void DynamicPointObject<TShader>::draw(RenderEngineInterface& engine)
{
	// update uniform buffer
	UniformBufferData uniformBufferData = {};
	updateUniformBuffer(uniformBufferData);
	{
		uint8_t * uniformBufferData_begin = reinterpret_cast<uint8_t *>(&uniformBufferData);
		uint8_t * uniformBufferData_end = uniformBufferData_begin + sizeof(updateUniformBuffer);
		auto data = std::span<uint8_t, sizeof(UniformBufferData)>(uniformBufferData_begin, uniformBufferData_end);

		engine.updateUniformBuffer(more.uniformBuffersMemory, data);
	}

	// update vertex buffer
	size_t const vertexBufferByteSize = more.mVertexBufferElementSize * more.mVertexBufferSize;
	engine.updateVertexBuffer(more.vertexBufferMemory, vertexBufferByteSize, 
		[&](void * data, size_t const size)
		{
			VertexBufferElement * vertexBufferBegin = reinterpret_cast<VertexBufferElement *>(data);
			VertexBufferElement * vertexBufferEnd = vertexBufferBegin + more.mVertexBufferSize;
			auto vertexData = std::span<VertexBufferElement>(vertexBufferBegin, vertexBufferEnd);
			updateVertexBuffer(vertexData, more.mFullBufferUpdateRequired);
		});
	
	// update storage buffer
	if (more.mStorageBufferSize > 0)
	{
		size_t const storageBufferByteSize = more.mStorageBufferElementSize * more.mStorageBufferSize;
		engine.updateVertexBuffer(more.storageBufferMemory, storageBufferByteSize,
		[&](void * data, size_t const size)
		{
			StorageBufferElement * storageBufferBegin = reinterpret_cast<StorageBufferElement *>(data);
			StorageBufferElement * storageBufferEnd = storageBufferBegin + more.mStorageBufferSize;
			auto storageData = std::span<StorageBufferElement>(storageBufferBegin, storageBufferEnd);
			updateStorageBuffer(storageData, more.mFullBufferUpdateRequired);
		});
	}

	more.mFullBufferUpdateRequired = false;
}

template<typename TShader>
inline void DynamicPointObject<TShader>::cleanup(RenderEngineInterface& engine) 
{
	more.cleanup(engine);
}

