///////////////////////////////////////////////////////////////////////////////
// File: HelloTriangleApplication.cpp
// Date: 21.08.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Draws a simple triangle
///////////////////////////////////////////////////////////////////////////////


#include "HelloTriangleApplication.h"

#include "File.h"

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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = upGLFWWindow(glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr));
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
	createGraphicsPipeline();
	createFramebuffers();
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
		vk::Extent2D acutalExtent = {static_cast<uint32_t>(WIDTH), static_cast<uint32_t>(HEIGHT) };

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

	renderPass = device->createRenderPassUnique(renderPassInfo);
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
	vertexInputInfo.setVertexBindingDescriptionCount(0);
	vertexInputInfo.setPVertexBindingDescriptions(nullptr);
	vertexInputInfo.setVertexAttributeDescriptionCount(0);
	vertexInputInfo.setPVertexAttributeDescriptions(nullptr);

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
	rasterizer.setFrontFace(vk::FrontFace::eClockwise);
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
	pipelineLayoutInfo.setSetLayoutCount(0); // Optional
	pipelineLayoutInfo.setPSetLayouts(nullptr); // Optional
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

void HelloTriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(window.get())) {
		glfwPollEvents();
	}
}

void HelloTriangleApplication::cleanup()
{

}

VKAPI_ATTR VkBool32 VKAPI_CALL HelloTriangleApplication::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		// Message is important enough to show
	}

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}
