//
// @file:   simple_pipeline.h
// @author: GrandChris
// @date:   2021-09-06
// @brief:  A default unifor buffer descriptor
//

#pragma once

#include "vulkan_erruption/shader_object/render_engine_interface.h"
#include "vulkan_erruption/object/simple_object/hello_triangle_shader.h"

#include <vulkan/vulkan.hpp>

class AdvancedGraphicsPipeline
{
public:
	void createGraphicsPipeline(RenderEngineInterface& engine, 
        std::vector<char> const & vertexShaderCode,
        std::vector<char> const & fragmentShaderCode,
        vk::VertexInputBindingDescription const & bindingDescription,
        std::vector<vk::VertexInputAttributeDescription>const& attributeDescriptions,
		vk::DescriptorSetLayout const & descriptorSetLayout
        );

    vk::PipelineLayout const & getPipelineLayout() const;
    vk::Pipeline const & getPipeline() const;

    void clear();
private:
	// Graphics pipeline
	vk::UniquePipelineLayout pipelineLayout;
	vk::UniquePipeline graphicsPipeline;

};





inline void AdvancedGraphicsPipeline::createGraphicsPipeline(RenderEngineInterface& engine, 
        std::vector<char> const & vertexShaderCode,
        std::vector<char> const & fragmentShaderCode,
        vk::VertexInputBindingDescription const & bindingDescription,
        std::vector<vk::VertexInputAttributeDescription>const& attributeDescriptions,
		vk::DescriptorSetLayout const & descriptorSetLayout
        )
{
    vk::Extent2D const swapChainExtent = engine.getSwapChainExtent();

    vk::UniqueShaderModule const vertShaderModule = engine.createShaderModule(vertexShaderCode);
	vk::UniqueShaderModule const fragShaderModule = engine.createShaderModule(fragmentShaderCode);

	vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
	vertShaderStageInfo.setStage(vk::ShaderStageFlagBits::eVertex);
	vertShaderStageInfo.setModule(vertShaderModule.get());
	vertShaderStageInfo.setPName("main");

	vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
	fragShaderStageInfo.setStage(vk::ShaderStageFlagBits::eFragment);
	fragShaderStageInfo.setModule(fragShaderModule.get());
	fragShaderStageInfo.setPName("main");

	size_t const shaderStagesSize = 2;
	vk::PipelineShaderStageCreateInfo shaderStages[shaderStagesSize] = 
    {
		vertShaderStageInfo,
		fragShaderStageInfo
    };

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.setVertexBindingDescriptionCount(1);
	vertexInputInfo.setPVertexBindingDescriptions(&bindingDescription);
	vertexInputInfo.setVertexAttributeDescriptionCount(static_cast<uint32_t>(attributeDescriptions.size()));
	vertexInputInfo.setPVertexAttributeDescriptions(attributeDescriptions.data());

	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	// inputAssembly.setTopology(vk::PrimitiveTopology::eTriangleList);
	inputAssembly.setTopology(vk::PrimitiveTopology::eTriangleStrip);
    // inputAssembly.setTopology(vk::PrimitiveTopology::ePointList);
	inputAssembly.setPrimitiveRestartEnable(VK_FALSE);

	vk::Viewport viewport;
	viewport.setX(0.0f);
	viewport.setY(0.0f);
	viewport.setWidth(swapChainExtent.width);
	viewport.setHeight(swapChainExtent.height);
	viewport.setMinDepth(0.0f);
	viewport.setMaxDepth(1.0f);

	vk::Rect2D scissor;
	scissor.setOffset({ 0, 0 });
	scissor.setExtent(swapChainExtent);

	vk::PipelineViewportStateCreateInfo viewportState;
	viewportState.setViewportCount(1);
	viewportState.setPViewports(&viewport);
	viewportState.setScissorCount(1);
	viewportState.setPScissors(&scissor);

	vk::PipelineRasterizationStateCreateInfo rasterizer;
	rasterizer.setDepthClampEnable(VK_FALSE);
	rasterizer.setRasterizerDiscardEnable(VK_FALSE);
	rasterizer.setPolygonMode(vk::PolygonMode::eFill);
	rasterizer.setLineWidth(1.0f);
	// rasterizer.setCullMode(vk::CullModeFlagBits::eBack);
	rasterizer.setCullMode(vk::CullModeFlagBits::eNone);
	//rasterizer.setFrontFace(vk::FrontFace::eClockwise);
	rasterizer.setFrontFace(vk::FrontFace::eCounterClockwise);
	//rasterizer.setFrontFace(vk::FrontFace::eClockwise);
	rasterizer.setDepthBiasEnable(VK_FALSE);
	rasterizer.setDepthBiasConstantFactor(0.0f); // Optional
	rasterizer.setDepthBiasClamp(0.0f); // Optional
	rasterizer.setDepthBiasSlopeFactor(0.0f); // Optional

    auto const msaaSamples = engine.getMaxMsaaSamples();
	vk::PipelineMultisampleStateCreateInfo multisampling;
	multisampling.setSampleShadingEnable(true);
	multisampling.setRasterizationSamples(msaaSamples);
	multisampling.setMinSampleShading(1.0f); // Optional
	multisampling.setPSampleMask(nullptr); // Optional
	multisampling.setAlphaToCoverageEnable(VK_FALSE); // Optional
	multisampling.setAlphaToOneEnable(VK_FALSE); // Optional

	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.setColorWriteMask(
		vk::ColorComponentFlagBits::eR |
		vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB |
		vk::ColorComponentFlagBits::eA
	);

	constexpr bool alphaBlending = false;
	if (alphaBlending)
	{
        colorBlendAttachment.setBlendEnable(VK_TRUE);
		colorBlendAttachment.setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha); // Optional
		colorBlendAttachment.setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha); // Optional
		colorBlendAttachment.setColorBlendOp(vk::BlendOp::eAdd); // Optional
		colorBlendAttachment.setSrcAlphaBlendFactor(vk::BlendFactor::eOne); // Optional
		colorBlendAttachment.setDstAlphaBlendFactor(vk::BlendFactor::eZero); // Optional
		colorBlendAttachment.setAlphaBlendOp(vk::BlendOp::eAdd); // Optional
    }
	else
	{
		colorBlendAttachment.setBlendEnable(VK_FALSE);
		colorBlendAttachment.setSrcColorBlendFactor(vk::BlendFactor::eOne); // Optional
		colorBlendAttachment.setDstColorBlendFactor(vk::BlendFactor::eZero); // Optional
		colorBlendAttachment.setColorBlendOp(vk::BlendOp::eAdd); // Optional
		colorBlendAttachment.setSrcAlphaBlendFactor(vk::BlendFactor::eOne); // Optional
		colorBlendAttachment.setDstAlphaBlendFactor(vk::BlendFactor::eZero); // Optional
		colorBlendAttachment.setAlphaBlendOp(vk::BlendOp::eAdd); // Optional
	}
	

	vk::PipelineColorBlendStateCreateInfo colorBlending;
	colorBlending.setLogicOpEnable(VK_FALSE);
	colorBlending.setLogicOp(vk::LogicOp::eCopy); // Optional
	colorBlending.setAttachmentCount(1);
	colorBlending.setPAttachments(&colorBlendAttachment);
	colorBlending.setBlendConstants({ 0.0f, 0.0f, 0.0f, 0.0f }); // Optional

	vk::DynamicState dynamicStates[] = {
		vk::DynamicState::eViewport,
		vk::DynamicState::eLineWidth
	};

	vk::PipelineDynamicStateCreateInfo dynamicState;
	dynamicState.setDynamicStateCount(2);
	dynamicState.setPDynamicStates(dynamicStates);

	vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.setSetLayoutCount(1);
	pipelineLayoutInfo.setPSetLayouts(&descriptorSetLayout);
	// pipelineLayoutInfo.setPushConstantRangeCount(0); // Optional
	// pipelineLayoutInfo.setPPushConstantRanges(nullptr); // Optional

	vk::PipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.setDepthTestEnable(VK_TRUE);
	depthStencil.setDepthWriteEnable(VK_TRUE);
	depthStencil.setDepthCompareOp(vk::CompareOp::eLess);
	depthStencil.setDepthBoundsTestEnable(VK_FALSE);
	depthStencil.setMinDepthBounds(0.0f); // Optional
	depthStencil.setMaxDepthBounds(1.0f); // Optional
	depthStencil.setStencilTestEnable(VK_FALSE);
	//depthStencil.setFront(); // Optional
	//depthStencil.setBack(); // Optional

	pipelineLayout = engine.getDevice().createPipelineLayoutUnique(pipelineLayoutInfo);

	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.setStageCount(static_cast<uint32_t>(shaderStagesSize));
	pipelineInfo.setPStages(shaderStages);
	pipelineInfo.setPVertexInputState(&vertexInputInfo);
	pipelineInfo.setPInputAssemblyState(&inputAssembly);
	pipelineInfo.setPViewportState(&viewportState);
	pipelineInfo.setPRasterizationState(&rasterizer);
	pipelineInfo.setPMultisampleState(&multisampling);
	pipelineInfo.setPDepthStencilState(&depthStencil); // optional
	pipelineInfo.setPColorBlendState(&colorBlending);
	pipelineInfo.setPDynamicState(nullptr); // optional
	pipelineInfo.setLayout(pipelineLayout.get());
	pipelineInfo.setRenderPass(engine.getRenderPass());
	pipelineInfo.setSubpass(0);
	//pipelineInfo.setBasePipelineHandle(); // Optional
	pipelineInfo.setBasePipelineIndex(-1); // Optional

	// graphicsPipeline = device->createGraphicsPipelineUnique(nullptr, pipelineInfo);
	graphicsPipeline = engine.getDevice().createGraphicsPipelineUnique(nullptr, pipelineInfo).value;
}

inline vk::PipelineLayout const & AdvancedGraphicsPipeline::getPipelineLayout() const
{
    return pipelineLayout.get();
}   

inline vk::Pipeline const & AdvancedGraphicsPipeline::getPipeline() const
{
    return graphicsPipeline.get();
}

inline void AdvancedGraphicsPipeline::clear() 
{
	graphicsPipeline.reset();
	pipelineLayout.reset();
}