///////////////////////////////////////////////////////////////////////////////
// File: HelloTriangleApplication.cpp
// Date: 21.08.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Draws a simple triangle
///////////////////////////////////////////////////////////////////////////////


#include "HelloTriangleApplication.h"

#include "File.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <map>
#include <set>

// runtime loaded vulkan functions

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
	VkInstance                                  instance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
	VkInstance                                  instance,
	VkDebugUtilsMessengerEXT                    messenger,
	const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, messenger, pAllocator);
	}
}

// <<---



void HelloTriangleApplication::run() 
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void HelloTriangleApplication::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = upGLFWWindow(glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr));
	glfwSetWindowUserPointer(window.get(), this);
	glfwSetFramebufferSizeCallback(window.get(), framebufferResizeCallback);
}

void HelloTriangleApplication::initVulkan()
{
	createInstance();
	setupDebugMessenger();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
	createImageViews();
	createRenderPass();
	createDescriptorSetLayout();
	createGraphicsPipeline();
	createFramebuffers();
	createCommandPool();
	createVertexBuffer();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	createSyncObjects();
}

void HelloTriangleApplication::createInstance()
{
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	vk::ApplicationInfo appInfo;
	appInfo.setPApplicationName("Hello Triangle");
	appInfo.setApplicationVersion(VK_MAKE_VERSION(1, 0, 0));
	appInfo.setPEngineName("No Engine");
	appInfo.setEngineVersion(VK_MAKE_VERSION(1, 0, 0));
	appInfo.setApiVersion(VK_API_VERSION_1_1);

	vk::InstanceCreateInfo createInfo;
	createInfo.setPApplicationInfo(&appInfo);

	vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (enableValidationLayers)
	{
		createInfo.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()));
		createInfo.setPpEnabledLayerNames(validationLayers.data());

		populateDebugMessengerCreateInfo(debugCreateInfo);
		debugCreateInfo.setMessageSeverity(
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
		);
		createInfo.setPNext(&debugCreateInfo);
	}

	auto const extension = getRequiredExtensions();
	createInfo.setEnabledExtensionCount(static_cast<uint32_t>(extension.size()));
	createInfo.setPpEnabledExtensionNames(extension.data());

	instance = vk::createInstanceUnique(createInfo);
}

std::vector<const char*> HelloTriangleApplication::getRequiredExtensions()
{
	// add extensions for glfw
	uint32_t glfwExtensionCount = 0;
	auto const glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	// add validation layer extension
	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool HelloTriangleApplication::checkValidationLayerSupport()
{
	auto const availableLayers = vk::enumerateInstanceLayerProperties();

	for (char const* layerName : validationLayers)
	{
		bool layerFound = false;

		for (auto const& layerPropberties : availableLayers)
		{
			if (strcmp(layerName, layerPropberties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

void HelloTriangleApplication::setupDebugMessenger()
{
	if (!enableValidationLayers)
	{
		return;
	}

	vk::DebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);
	
	debugMessanger = instance->createDebugUtilsMessengerEXTUnique(createInfo);
}

void HelloTriangleApplication::populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo.setMessageSeverity(
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
	);
	createInfo.setMessageType(
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
		vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
	);
	createInfo.setPfnUserCallback(debugCallback);
}

void HelloTriangleApplication::createSurface()
{
	VkSurfaceKHR pSurface;

	if (glfwCreateWindowSurface(instance.get(), window.get(), nullptr, &pSurface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	surface = vk::UniqueSurfaceKHR(pSurface, instance.get());
}

void HelloTriangleApplication::pickPhysicalDevice()
{
	auto const devices = instance->enumeratePhysicalDevices();

	// Use an ordered map to automatically sort candidates by increasing score
	std::multimap<int, vk::PhysicalDevice const &> candidates;

	for (auto const& device : devices)
	{
		int const score = rateDeviceSuitability(device);
		candidates.insert(std::make_pair(score, device));
	}

	if (!candidates.empty() && candidates.rbegin()->first > 0 && isDeviceSuitable(candidates.rbegin()->second))
	{
		physicalDevice = candidates.rbegin()->second;
	}
	else
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

int HelloTriangleApplication::rateDeviceSuitability(vk::PhysicalDevice const& device)
{
	auto const deviceProperties = device.getProperties();
	auto const deviceFeatures = device.getFeatures();

	int score = 0;

	if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
	{
		score += 1000;
	}

	// Maximum possible size of textures affects graphics quality
	score += deviceProperties.limits.maxImageDimension2D;

	// Application can't function without geometry shaders
	if (!deviceFeatures.geometryShader) {
		return 0;
	}

	return score;
}

bool HelloTriangleApplication::isDeviceSuitable(vk::PhysicalDevice const& device)
{
	QueueFamilyIndices const indices = findQueueFamilies(device);

	bool const extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

HelloTriangleApplication::QueueFamilyIndices HelloTriangleApplication::findQueueFamilies(vk::PhysicalDevice const& device)
{
	QueueFamilyIndices indices;

	auto const queueFamilies = device.getQueueFamilyProperties();

	int i = 0;
	for (auto const& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.graphicsFamily = i;
		}

		VkBool32 const presentSupport = device.getSurfaceSupportKHR(i, surface.get());
		if (queueFamily.queueCount > 0 && presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

void HelloTriangleApplication::createLogicalDevice()
{
	QueueFamilyIndices const indices = findQueueFamilies(physicalDevice);

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueuFamilies = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};

	float const queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueuFamilies)
	{
		vk::DeviceQueueCreateInfo queueCreateInfo;
		queueCreateInfo.setQueueFamilyIndex(indices.graphicsFamily.value());
		queueCreateInfo.setQueueCount(1);
		queueCreateInfo.setPQueuePriorities(&queuePriority);
		
		queueCreateInfos.push_back(queueCreateInfo);
	}
	
	vk::PhysicalDeviceFeatures deviceFeatures;

	vk::DeviceCreateInfo createInfo;
	createInfo.setQueueCreateInfoCount(static_cast<uint32_t>(queueCreateInfos.size()));
	createInfo.setPQueueCreateInfos(queueCreateInfos.data());
	
	createInfo.setPEnabledFeatures(&deviceFeatures);

	createInfo.setEnabledExtensionCount(static_cast<uint32_t>(deviceExtensions.size()));
	createInfo.setPpEnabledExtensionNames(deviceExtensions.data());

	device = physicalDevice.createDeviceUnique(createInfo);

	graphicsQueue =  device->getQueue(indices.graphicsFamily.value(), 0);
	presentQueue = device->getQueue(indices.presentFamily.value(), 0);
}

bool HelloTriangleApplication::checkDeviceExtensionSupport(vk::PhysicalDevice const & device)
{
	auto const availableExtensions = device.enumerateDeviceExtensionProperties();

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void HelloTriangleApplication::createSwapChain()
{
	SwapChainSupportDetails const swapChainSupport = querySwapChainSupport(physicalDevice);

	auto const surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	auto const presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	auto const extent = chooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	vk::SwapchainCreateInfoKHR createInfo;
	createInfo.setSurface(surface.get());
	createInfo.setMinImageCount(imageCount);
	createInfo.setImageFormat(surfaceFormat.format);
	createInfo.setImageColorSpace(surfaceFormat.colorSpace);
	createInfo.setImageExtent(extent);
	createInfo.setImageArrayLayers(1);
	createInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

	QueueFamilyIndices const indices = findQueueFamilies(physicalDevice);
	uint32_t const queueFamilyIndices[] = { 
		indices.graphicsFamily.value(), 
		indices.presentFamily.value() 
	};

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
		createInfo.setQueueFamilyIndexCount(2);
		createInfo.setPQueueFamilyIndices(queueFamilyIndices);
	}
	else
	{
		createInfo.setImageSharingMode(vk::SharingMode::eExclusive);
		createInfo.setQueueFamilyIndexCount(0);	// Optional
		createInfo.setPQueueFamilyIndices(nullptr); // Optional
	}

	createInfo.setPreTransform(swapChainSupport.capabilities.currentTransform);
	createInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
	createInfo.setPresentMode(presentMode);
	createInfo.setClipped(VK_TRUE);

	swapChain = device->createSwapchainKHRUnique(createInfo);

	swapChainImages = device->getSwapchainImagesKHR(swapChain.get());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

HelloTriangleApplication::SwapChainSupportDetails HelloTriangleApplication::querySwapChainSupport(vk::PhysicalDevice const & device)
{
	SwapChainSupportDetails details;

	details.capabilities = device.getSurfaceCapabilitiesKHR(surface.get());
	details.formats = device.getSurfaceFormatsKHR(surface.get());
	details.presentModes = device.getSurfacePresentModesKHR(surface.get());

	return details;
}

vk::SurfaceFormatKHR HelloTriangleApplication::chooseSwapSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const& availableFormats)
{
	for (auto const& availableFormat : availableFormats)
	{
		if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
			availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
		{
			return availableFormat;
		}
	}

	return availableFormats.front();
}

vk::PresentModeKHR HelloTriangleApplication::chooseSwapPresentMode(std::vector<vk::PresentModeKHR> const& availablePresentModes)
{
	for (auto const& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == vk::PresentModeKHR::eMailbox)
		{
			return availablePresentMode;
		}
	}

	return vk::PresentModeKHR::eFifo;
}

vk::Extent2D HelloTriangleApplication::chooseSwapExtent(vk::SurfaceCapabilitiesKHR const & capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width = 0;
		int height = 0;
		glfwGetFramebufferSize(window.get(), &width, &height);

		vk::Extent2D acutalExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

		acutalExtent.setWidth(
			std::max(
				capabilities.minImageExtent.width,
				std::min(capabilities.maxImageExtent.width, acutalExtent.width)
			));

		acutalExtent.setHeight(
			std::max(
				capabilities.minImageExtent.height,
				std::min(capabilities.maxImageExtent.height, acutalExtent.height)
			));

		return acutalExtent;
	}
}

void HelloTriangleApplication::createImageViews()
{
	//swapChainImageViews.resize(swapChainImages.size());

	for (auto const& swapChaninImage : swapChainImages)
	{
		vk::ImageViewCreateInfo createInfo;
		createInfo.setImage(swapChaninImage);
		createInfo.setViewType(vk::ImageViewType::e2D);
		createInfo.setFormat(swapChainImageFormat);

		createInfo.components.setR(vk::ComponentSwizzle::eIdentity);
		createInfo.components.setG(vk::ComponentSwizzle::eIdentity);
		createInfo.components.setB(vk::ComponentSwizzle::eIdentity);
		createInfo.components.setA(vk::ComponentSwizzle::eIdentity);

		createInfo.subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor);
		createInfo.subresourceRange.setBaseMipLevel(0);
		createInfo.subresourceRange.setLevelCount(1);
		createInfo.subresourceRange.setBaseArrayLayer(0);
		createInfo.subresourceRange.setLayerCount(1);

		swapChainImageViews.push_back(device->createImageViewUnique(createInfo));
	}


}

void HelloTriangleApplication::createRenderPass()
{
	vk::AttachmentDescription colorAttachment;
	colorAttachment.setFormat(swapChainImageFormat);
	colorAttachment.setSamples(vk::SampleCountFlagBits::e1);
	colorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
	colorAttachment.setStoreOp(vk::AttachmentStoreOp::eStore);
	colorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
	colorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
	colorAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
	colorAttachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

	vk::AttachmentReference colorAttachmentRef;
	colorAttachmentRef.setAttachment(0);
	colorAttachmentRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

	vk::SubpassDescription subpass;
	subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
	subpass.setColorAttachmentCount(1);
	subpass.setPColorAttachments(&colorAttachmentRef);

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.setAttachmentCount(1);
	renderPassInfo.setPAttachments(&colorAttachment);
	renderPassInfo.setSubpassCount(1);
	renderPassInfo.setPSubpasses(&subpass);


	vk::SubpassDependency dependency;
	dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL);
	dependency.setDstSubpass(0);
	dependency.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
	//dependency.setSrcAccessMask(0);
	dependency.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
	dependency.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);
	
	renderPassInfo.setDependencyCount(1);
	renderPassInfo.setPDependencies(&dependency);


	renderPass = device->createRenderPassUnique(renderPassInfo);
}

void HelloTriangleApplication::createDescriptorSetLayout()
{
	vk::DescriptorSetLayoutBinding uboLayoutBinding;
	uboLayoutBinding.setBinding(0);
	uboLayoutBinding.setDescriptorType(vk::DescriptorType::eUniformBuffer);
	uboLayoutBinding.setDescriptorCount(1);
	uboLayoutBinding.setStageFlags(vk::ShaderStageFlagBits::eVertex);
	uboLayoutBinding.setPImmutableSamplers(nullptr); // Optional
	
	vk::DescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.setBindingCount(1);
	layoutInfo.setPBindings(&uboLayoutBinding);

	descriptorSetLayout = device->createDescriptorSetLayoutUnique(layoutInfo);	
}

void HelloTriangleApplication::createGraphicsPipeline()
{
	auto const vertShaderCode = readFile("shaders/vert.spv");
	auto const fragShaderCode = readFile("shaders/frag.spv");

	auto const vertShaderModule = createShaderModule(vertShaderCode);
	auto const fragShaderModule = createShaderModule(fragShaderCode);

	vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
	vertShaderStageInfo.setStage(vk::ShaderStageFlagBits::eVertex);
	vertShaderStageInfo.setModule(vertShaderModule.get());
	vertShaderStageInfo.setPName("main");

	vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
	fragShaderStageInfo.setStage(vk::ShaderStageFlagBits::eFragment);
	fragShaderStageInfo.setModule(fragShaderModule.get());
	fragShaderStageInfo.setPName("main");

	vk::PipelineShaderStageCreateInfo const shaderStages[] = {
		vertShaderStageInfo,
		fragShaderStageInfo
	};

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.setVertexBindingDescriptionCount(1);
	vertexInputInfo.setPVertexBindingDescriptions(&bindingDescription);
	vertexInputInfo.setVertexAttributeDescriptionCount(static_cast<uint32_t>(attributeDescriptions.size()));
	vertexInputInfo.setPVertexAttributeDescriptions(attributeDescriptions.data());

	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.setTopology(vk::PrimitiveTopology::eTriangleList);
	inputAssembly.setPrimitiveRestartEnable(VK_FALSE);

	vk::Viewport viewport;
	viewport.setX(0.0f);
	viewport.setY(0.0f);
	viewport.setWidth(static_cast<float>(swapChainExtent.width));
	viewport.setHeight(static_cast<float>(swapChainExtent.height));
	viewport.setMaxDepth(0.0f);
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
	rasterizer.setCullMode(vk::CullModeFlagBits::eBack);
	rasterizer.setFrontFace(vk::FrontFace::eCounterClockwise);
	rasterizer.setDepthBiasEnable(VK_FALSE);
	rasterizer.setDepthBiasConstantFactor(0.0f); // Optional
	rasterizer.setDepthBiasClamp(0.0f); // Optional
	rasterizer.setDepthBiasSlopeFactor(0.0f); // Optional

	vk::PipelineMultisampleStateCreateInfo multisampling;
	multisampling.setSampleShadingEnable(VK_FALSE);
	multisampling.setRasterizationSamples(vk::SampleCountFlagBits::e1);
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
	if constexpr (!alphaBlending)
	{
		colorBlendAttachment.setBlendEnable(VK_FALSE);
		colorBlendAttachment.setSrcColorBlendFactor(vk::BlendFactor::eOne); // Optional
		colorBlendAttachment.setDstColorBlendFactor(vk::BlendFactor::eZero); // Optional
		colorBlendAttachment.setColorBlendOp(vk::BlendOp::eAdd); // Optional
		colorBlendAttachment.setSrcAlphaBlendFactor(vk::BlendFactor::eOne); // Optional
		colorBlendAttachment.setDstAlphaBlendFactor(vk::BlendFactor::eZero); // Optional
		colorBlendAttachment.setAlphaBlendOp(vk::BlendOp::eAdd); // Optional
	}
	else
	{
		colorBlendAttachment.setBlendEnable(VK_TRUE);
		colorBlendAttachment.setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha); // Optional
		colorBlendAttachment.setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha); // Optional
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
	pipelineLayoutInfo.setPSetLayouts(&descriptorSetLayout.get()); 
	pipelineLayoutInfo.setPushConstantRangeCount(0); // Optional
	pipelineLayoutInfo.setPPushConstantRanges(nullptr); // Optional

	pipelineLayout = device->createPipelineLayoutUnique(pipelineLayoutInfo);

	vk::GraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.setStageCount(2);
	pipelineInfo.setPStages(shaderStages);
	pipelineInfo.setPVertexInputState(&vertexInputInfo);
	pipelineInfo.setPInputAssemblyState(&inputAssembly);
	pipelineInfo.setPViewportState(&viewportState);
	pipelineInfo.setPRasterizationState(&rasterizer);
	pipelineInfo.setPMultisampleState(&multisampling);
	pipelineInfo.setPDepthStencilState(nullptr); // optional
	pipelineInfo.setPColorBlendState(&colorBlending);
	pipelineInfo.setPDynamicState(nullptr); // optional
	pipelineInfo.setLayout(pipelineLayout.get());
	pipelineInfo.setRenderPass(renderPass.get());
	pipelineInfo.setSubpass(0);
	//pipelineInfo.setBasePipelineHandle(); // Optional
	pipelineInfo.setBasePipelineIndex(-1); // Optional

	graphicsPipeline = device->createGraphicsPipelineUnique(nullptr, pipelineInfo);
}

vk::UniqueShaderModule HelloTriangleApplication::createShaderModule(std::vector<char> const& code)
{
	vk::ShaderModuleCreateInfo createInfo;
	createInfo.setCodeSize(code.size());
	createInfo.setPCode(reinterpret_cast<uint32_t const*>(code.data()));

	auto shaderModule = device->createShaderModuleUnique(createInfo);

	return shaderModule;
}

void HelloTriangleApplication::createFramebuffers()
{
	for (auto const& swapImageView : swapChainImageViews)
	{
		vk::ImageView attachments[] = {
			swapImageView.get()
		};

		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.setRenderPass(renderPass.get());
		framebufferInfo.setAttachmentCount(1);
		framebufferInfo.setPAttachments(attachments);
		framebufferInfo.setWidth(swapChainExtent.width);
		framebufferInfo.setHeight(swapChainExtent.height);
		framebufferInfo.setLayers(1);

		swapChainFramebuffers.push_back(device->createFramebufferUnique(framebufferInfo));
	}
}

void HelloTriangleApplication::createCommandPool()
{
	QueueFamilyIndices const queueFamilyIndices = findQueueFamilies(physicalDevice);

	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.setQueueFamilyIndex(queueFamilyIndices.graphicsFamily.value());
	//poolInfo.setFlags( ); // Optional

	commandPool = device->createCommandPoolUnique(poolInfo);
}

void HelloTriangleApplication::createVertexBuffer()
{
	vk::DeviceSize const bufferSize = sizeof(vertices.front()) * vertices.size();

	vk::UniqueDeviceMemory stagingBufferMemory;
	vk::UniqueBuffer stagingBuffer;

	createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
		stagingBuffer, stagingBufferMemory
	);

	auto const data = device->mapMemory(stagingBufferMemory.get(), 0, bufferSize);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	device->unmapMemory(stagingBufferMemory.get());

	createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		vertexBuffer, vertexBufferMemory
	);

	copyBuffer(stagingBuffer.get(), vertexBuffer.get(), bufferSize);
}

uint32_t HelloTriangleApplication::findMemoryType(uint32_t const typeFilter, vk::MemoryPropertyFlags const& properties)
{
	auto const memProperties = physicalDevice.getMemoryProperties();

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i)) &&
			(memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");

	return uint32_t();
}

void HelloTriangleApplication::createBuffer(vk::DeviceSize const size, 
	vk::BufferUsageFlags const usage, vk::MemoryPropertyFlags const properties, 
	vk::UniqueBuffer & buffer, vk::UniqueDeviceMemory & bufferMemory)
{
	vk::BufferCreateInfo bufferInfo;
	bufferInfo.setSize(size);
	bufferInfo.setUsage(usage);
	bufferInfo.setSharingMode(vk::SharingMode::eExclusive);

	buffer = device->createBufferUnique(bufferInfo);

	auto const memRequirements = device->getBufferMemoryRequirements(buffer.get());

	vk::MemoryAllocateInfo allocInfo;
	allocInfo.setAllocationSize(memRequirements.size);
	allocInfo.setMemoryTypeIndex(findMemoryType(memRequirements.memoryTypeBits, properties));

	bufferMemory = device->allocateMemoryUnique(allocInfo);

	device->bindBufferMemory(buffer.get(), bufferMemory.get(), 0);
}

void HelloTriangleApplication::copyBuffer(vk::Buffer const& srcBuffer, vk::Buffer& dstBuffer, vk::DeviceSize size)
{
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.setLevel(vk::CommandBufferLevel::ePrimary);
	allocInfo.setCommandPool(commandPool.get());
	allocInfo.setCommandBufferCount(1);

	auto commandBuffer = device->allocateCommandBuffersUnique(allocInfo);

	vk::CommandBufferBeginInfo beginInfo;
	beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

	commandBuffer.front()->begin(beginInfo);
		vk::BufferCopy copyRegion;
		copyRegion.setSrcOffset(0); // Optional
		copyRegion.setDstOffset(0); // Optional
		copyRegion.setSize(size);
		commandBuffer.front()->copyBuffer(srcBuffer, dstBuffer, copyRegion);
	commandBuffer.front()->end();

	vk::SubmitInfo submitInfo;
	submitInfo.setCommandBufferCount(1);
	vk::CommandBuffer pCommandBuffers[] = { commandBuffer.front().get() };
	submitInfo.setPCommandBuffers(pCommandBuffers);

	graphicsQueue.submit(submitInfo, nullptr);
	graphicsQueue.waitIdle();
}

void HelloTriangleApplication::createUniformBuffers()
{
	vk::DeviceSize const bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(swapChainImages.size());
	uniformBuffersMemory.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); ++i)
	{
		createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
			uniformBuffers[i], uniformBuffersMemory[i]
		);
	}
}

void HelloTriangleApplication::createDescriptorPool()
{
	vk::DescriptorPoolSize poolSize;
	poolSize.setDescriptorCount(static_cast<uint32_t>(swapChainImages.size()));

	vk::DescriptorPoolCreateInfo poolInfo;
	poolInfo.setPoolSizeCount(1);
	poolInfo.setPPoolSizes(&poolSize);
	poolInfo.setMaxSets(static_cast<uint32_t>(swapChainImages.size()));

	descriptorPool = device->createDescriptorPoolUnique(poolInfo);
}

void HelloTriangleApplication::createDescriptorSets()
{
	std::vector<vk::DescriptorSetLayout> const layouts(swapChainImages.size(), descriptorSetLayout.get());

	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.setDescriptorPool(descriptorPool.get());
	allocInfo.setDescriptorSetCount(static_cast<uint32_t>(swapChainImages.size()));
	allocInfo.setPSetLayouts(layouts.data());

	descriptorSets = device->allocateDescriptorSets(allocInfo);

	for (size_t i = 0; i < swapChainImages.size(); i++) 
	{
		vk::DescriptorBufferInfo bufferInfo;
		bufferInfo.setBuffer(uniformBuffers[i].get());
		bufferInfo.setOffset(0);
		bufferInfo.setRange(sizeof(UniformBufferObject));

		vk::WriteDescriptorSet descriptorWrite;
		descriptorWrite.setDstSet(descriptorSets[i]);
		descriptorWrite.setDstBinding(0);
		descriptorWrite.setDstArrayElement(0);
		descriptorWrite.setDescriptorType(vk::DescriptorType::eUniformBuffer);
		descriptorWrite.setDescriptorCount(1);
		descriptorWrite.setPBufferInfo(&bufferInfo);
		descriptorWrite.setPImageInfo(nullptr); // Optional
		descriptorWrite.setPTexelBufferView(nullptr); // Optional

		device->updateDescriptorSets(descriptorWrite, nullptr);
	}
}

void HelloTriangleApplication::createCommandBuffers()
{
	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.setCommandPool(commandPool.get());
	allocInfo.setLevel(vk::CommandBufferLevel::ePrimary);
	allocInfo.setCommandBufferCount(static_cast<uint32_t>(swapChainFramebuffers.size()));

	commandBuffers = device->allocateCommandBuffersUnique(allocInfo);

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		vk::CommandBufferBeginInfo beginInfo;
		//beginInfo.setFlags( ); // Optional
		beginInfo.setPInheritanceInfo(nullptr); // Optional

		commandBuffers[i]->begin(beginInfo);

			vk::RenderPassBeginInfo renderPassInfo;
			renderPassInfo.setRenderPass(renderPass.get());
			renderPassInfo.setFramebuffer(swapChainFramebuffers[i].get());
			renderPassInfo.renderArea.setOffset({ 0, 0 });
			renderPassInfo.renderArea.setExtent(swapChainExtent);
			vk::ClearValue const clearColor(std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f });
			renderPassInfo.setClearValueCount(1);
			renderPassInfo.setPClearValues(&clearColor);

			commandBuffers[i]->beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

				commandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline.get());

				//vk::ArrayProxy<vk::Buffer const> vertexBuffers = { vertexBuffer.get() };
				//vk::ArrayProxy<vk::DeviceSize const> offsets = { 0 };
				//commandBuffers[i]->bindVertexBuffers(0, vertexBuffers, offsets);	// fails on release build
				commandBuffers[i]->bindVertexBuffers(0, vertexBuffer.get(), vk::DeviceSize());

				commandBuffers[i]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSets[i], nullptr);

				commandBuffers[i]->draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0);

			commandBuffers[i]->endRenderPass();

		commandBuffers[i]->end();
	}
}

void HelloTriangleApplication::createSyncObjects()
{
	vk::SemaphoreCreateInfo semaphoreInfo;
	vk::FenceCreateInfo fenceInfo;
	fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) 
	{
		imageAvailableSemaphores.push_back(device->createSemaphoreUnique(semaphoreInfo));
		renderFinishedSemaphores.push_back(device->createSemaphoreUnique(semaphoreInfo));
		inFlightFences.push_back(device->createFenceUnique(fenceInfo));
	}	
}

void HelloTriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(window.get())) {
		glfwPollEvents();
		drawFrame();
	}

	device->waitIdle();
}

void HelloTriangleApplication::drawFrame()
{
	device->waitForFences(inFlightFences[currentFrame].get(), VK_TRUE, UINT64_MAX);
	
	vk::ResultValue<uint32_t> res(vk::Result::eSuccess, 0);
	try {
		res = device->acquireNextImageKHR(swapChain.get(), UINT64_MAX, imageAvailableSemaphores[currentFrame].get(), nullptr);
		if (res.result == vk::Result::eErrorOutOfDateKHR)	// window was resized, swap chain is now incompatible
		{
			recreateSwapChain();
			return;
		}
		else if (res.result != vk::Result::eSuccess && res.result != vk::Result::eSuboptimalKHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}
	}
	catch (vk::OutOfDateKHRError const &)
	{
		recreateSwapChain();
		return;
	}
	
	uint32_t const imageIndex = res.value;

	updateUniformBuffer(imageIndex);

	vk::SubmitInfo submitInfo;
	
	vk::Semaphore waitSemaphore[] = { imageAvailableSemaphores[currentFrame].get() };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	submitInfo.setWaitSemaphoreCount(1);
	submitInfo.setPWaitSemaphores(waitSemaphore);
	submitInfo.setPWaitDstStageMask(waitStages);
	submitInfo.setCommandBufferCount(1);
	submitInfo.setPCommandBuffers(&commandBuffers[imageIndex].get());

	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame].get() };
	submitInfo.setSignalSemaphoreCount(1);
	submitInfo.setPSignalSemaphores(signalSemaphores);

	device->resetFences(inFlightFences[currentFrame].get());

	graphicsQueue.submit(submitInfo, inFlightFences[currentFrame].get());

	vk::PresentInfoKHR presentInfo;
	presentInfo.setWaitSemaphoreCount(1);
	presentInfo.setPWaitSemaphores(signalSemaphores);
	vk::SwapchainKHR swapChains[] = { swapChain.get() };
	presentInfo.setSwapchainCount(1);
	presentInfo.setPSwapchains(swapChains);
	presentInfo.setPImageIndices(&imageIndex);
	presentInfo.setPResults(nullptr); // optional

	try {
		auto const resPresent = presentQueue.presentKHR(presentInfo);
		if (resPresent == vk::Result::eErrorOutOfDateKHR || resPresent == vk::Result::eSuboptimalKHR || framebufferResized)	// window was resized, swap chain is now incompatible
		{
			framebufferResized = false;
			recreateSwapChain();
		}
		else if (res.result != vk::Result::eSuccess) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}
	}
	catch (vk::OutOfDateKHRError const & )
	{
		framebufferResized = false;
		recreateSwapChain();
	}


	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage)
{
	static auto const startTime = std::chrono::high_resolution_clock::now();

	auto const currentTime = std::chrono::high_resolution_clock::now();

	float const time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo;
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1; // invert Y for Vulkan

	auto const data = device->mapMemory(uniformBuffersMemory[currentImage].get(), 0, sizeof(ubo));
	memcpy(data, &ubo, static_cast<size_t>(sizeof(ubo)));
	device->unmapMemory(uniformBuffersMemory[currentImage].get());
}

void HelloTriangleApplication::cleanup()
{

}

void HelloTriangleApplication::recreateSwapChain()
{
	int width = 0, height = 0;
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window.get(), &width, &height);
		glfwWaitEvents();
	}

	device->waitIdle();

	cleanupSwapChain();

	createSwapChain();
	createImageViews();
	createRenderPass();
	createGraphicsPipeline();
	createFramebuffers();
	createUniformBuffers();
	createDescriptorPool();
	createDescriptorSets();
	createCommandBuffers();
	
}

void HelloTriangleApplication::cleanupSwapChain()
{
	swapChainFramebuffers.clear();
	descriptorSets.clear();
	descriptorPool.reset();	
	commandBuffers.clear();
	uniformBuffers.clear();
	uniformBuffersMemory.clear();
	graphicsPipeline.reset();
	pipelineLayout.reset();
	renderPass.reset();
	swapChainImageViews.clear();
	swapChain.reset();
}

VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApplication::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		// Message is important enough to show
	}

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void HelloTriangleApplication::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}
