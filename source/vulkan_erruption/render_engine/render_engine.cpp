///////////////////////////////////////////////////////////////////////////////
// File: HelloTriangleApplication.cpp
// Date: 21.08.2019
// Version: 1
// Author: Christian Steinbrecher
// Description: Draws a simple triangle
///////////////////////////////////////////////////////////////////////////////


#include "vulkan_erruption/render_engine/render_engine.h"

//#include "shaders/shader_vert_spv.h"
//#include "shaders/shader_frag_spv.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <map>
#include <set>
#include <mutex>

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


// trace marker

VKAPI_ATTR void  VKAPI_CALL vkCmdBeginDebugUtilsLabelEXT(
	VkInstance                  instance,
	VkCommandBuffer             commandBuffer,
	const VkDebugUtilsLabelEXT  & MarkerInfo
)
{
	auto func = (PFN_vkCmdBeginDebugUtilsLabelEXT)vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT");
	if (func != nullptr) {
		func(commandBuffer, &MarkerInfo);
	}
}


VKAPI_ATTR void VKAPI_CALL vkCmdEndDebugUtilsLabelEXT(
	VkInstance                  instance,
	VkCommandBuffer             commandBuffer
)
{
	auto func = (PFN_vkCmdEndDebugUtilsLabelEXT)vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT");
	if (func != nullptr) {
		func(commandBuffer);
	}
}

// <<---


// Helper functions

static Key GLFWKeyToKey(int const glfwKey) {
	switch (glfwKey) {
	case GLFW_KEY_A: return Key::A;
	case GLFW_KEY_B: return Key::B;
	case GLFW_KEY_C: return Key::C;
	case GLFW_KEY_D: return Key::D;
	case GLFW_KEY_E: return Key::E;
	case GLFW_KEY_F: return Key::F;
	case GLFW_KEY_G: return Key::G;
	case GLFW_KEY_H: return Key::H;
	case GLFW_KEY_I: return Key::I;
	case GLFW_KEY_J: return Key::J;
	case GLFW_KEY_K: return Key::K;
	case GLFW_KEY_L: return Key::L;
	case GLFW_KEY_M: return Key::M;
	case GLFW_KEY_N: return Key::N;
	case GLFW_KEY_O: return Key::O;
	case GLFW_KEY_P: return Key::P;
	case GLFW_KEY_Q: return Key::Q;
	case GLFW_KEY_R: return Key::R;
	case GLFW_KEY_S: return Key::S;
	case GLFW_KEY_T: return Key::T;
	case GLFW_KEY_U: return Key::U;
	case GLFW_KEY_V: return Key::V;
	case GLFW_KEY_W: return Key::W;
	case GLFW_KEY_X: return Key::X;
	case GLFW_KEY_Y: return Key::Y;

	case GLFW_KEY_KP_0: return Key::Num0;
	case GLFW_KEY_KP_1: return Key::Num1;
	case GLFW_KEY_KP_2: return Key::Num2;
	case GLFW_KEY_KP_3: return Key::Num3;
	case GLFW_KEY_KP_4: return Key::Num4;
	case GLFW_KEY_KP_5: return Key::Num5;
	case GLFW_KEY_KP_6: return Key::Num6;
	case GLFW_KEY_KP_7: return Key::Num7;
	case GLFW_KEY_KP_8: return Key::Num8;
	case GLFW_KEY_KP_9: return Key::Num9;

	default: return Key::Null;
	}
}


// <<---


// ParticleRenderer::uPtr ParticleRenderer::createVulkan(bool enableValidationLayer)
// {
// 	return std::make_unique<RenderEngine>(enableValidationLayer);
// }


RenderEngine::RenderEngine(bool enableValidationLayer)
	: enableValidationLayers(enableValidationLayer)
{
	static std::mutex mtx;

	mtx.lock();
	initWindow();	// glfw does not support mutlithreading
	mtx.unlock();

	initVulkan();
}

RenderEngine::~RenderEngine()
{
	for(auto & elem: mObjs) {
		elem.get().cleanup(*this);	// must be done before deleting the other resources
	}

	mObjs.clear();	
}

void RenderEngine::add(ShaderObject & shaderObject)
{
	shaderObject.setup(*this);
	mObjs.push_back(shaderObject);
}

void RenderEngine::setup(ShaderObject & obj)
{
	obj.setup(*this);
}

void RenderEngine::draw(ShaderObject & obj, size_t const imageIndex)
{
	obj.draw(*this, imageIndex);
}

void RenderEngine::cleanup(ShaderObject & obj)
{
	obj.cleanup(*this);
}



void RenderEngine::run() 
{
	createCommandBuffersEnd();
	mainLoop();
	cleanup();
}


void RenderEngine::initWindow()
{
	glfwInit();	

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if (mFullScreenEnabled == false)
	{
		window = upGLFWWindow(glfwCreateWindow(static_cast<int>(mWidth), static_cast<int>(mHeight), "Vulkan", nullptr, nullptr));
	}
	else
	{
		window = upGLFWWindow(glfwCreateWindow(static_cast<int>(mWidth), static_cast<int>(mHeight), "Vulkan", glfwGetPrimaryMonitor(), nullptr));
	}
	
	glfwSetWindowUserPointer(window.get(), this);
	glfwSetFramebufferSizeCallback(window.get(), framebufferResizeCallback);
	glfwSetKeyCallback(window.get(), keyInputCallback);


	//glfwMaximizeWindow(window.get());
}

void RenderEngine::initVulkan()
{
	createInstance();
	setupDebugMessenger();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
	createImageViews();
	createRenderPass();
	//createDescriptorSetLayout();
	//createGraphicsPipeline();
	createCommandPool();
	createColorResources();
	createDepthResources();
	createFramebuffers();
	//createVertexBuffer();
	//createUniformBuffers();
	//createDescriptorPool();
	//createDescriptorSets();
	createCommandBuffersBegin();
	createSyncObjects();
}

void RenderEngine::createInstance()
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

std::vector<const char*> RenderEngine::getRequiredExtensions()
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

bool RenderEngine::checkValidationLayerSupport()
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

void RenderEngine::setupDebugMessenger()
{
	if (!enableValidationLayers)
	{
		return;
	}

	vk::DebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);
	
	debugMessanger = instance->createDebugUtilsMessengerEXTUnique(createInfo);
}

void RenderEngine::populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo)
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

void RenderEngine::createSurface()
{
	VkSurfaceKHR pSurface;

	if (glfwCreateWindowSurface(instance.get(), window.get(), nullptr, &pSurface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}

	surface = vk::UniqueSurfaceKHR(pSurface, instance.get());
}

void RenderEngine::pickPhysicalDevice()
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

		// Get MSAA count
		vk::SampleCountFlagBits const maxMsaaSamples = getMaxUsableSampleCount();
		#undef min
		msaaSamples = std::min(msaaSamplesPrefered, maxMsaaSamples);
	}
	else
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

int RenderEngine::rateDeviceSuitability(vk::PhysicalDevice const& device)
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

bool RenderEngine::isDeviceSuitable(vk::PhysicalDevice const& device)
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

RenderEngine::QueueFamilyIndices RenderEngine::findQueueFamilies(vk::PhysicalDevice const& device)
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

vk::SampleCountFlagBits RenderEngine::getMaxUsableSampleCount()
{
	assert(physicalDevice);

	vk::PhysicalDeviceProperties const physicalDeviceProperties = physicalDevice.getProperties();

	vk::SampleCountFlags const counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & 
								        physicalDeviceProperties.limits.framebufferDepthSampleCounts;
	
	if (counts & vk::SampleCountFlagBits::e64) {
		return   vk::SampleCountFlagBits::e64;
	}
	else if (counts & vk::SampleCountFlagBits::e32) {
		return        vk::SampleCountFlagBits::e32;
	}
	else if (counts & vk::SampleCountFlagBits::e16) {
		return        vk::SampleCountFlagBits::e16;
	}
	else if (counts & vk::SampleCountFlagBits::e8) {
		return        vk::SampleCountFlagBits::e8;
	}
	else if (counts & vk::SampleCountFlagBits::e4) {
		return        vk::SampleCountFlagBits::e4;
	}
	else if (counts & vk::SampleCountFlagBits::e2) {
		return        vk::SampleCountFlagBits::e2;
	}
	else {
		return vk::SampleCountFlagBits::e1;
	}
}

void RenderEngine::createLogicalDevice()
{
	QueueFamilyIndices const indices = findQueueFamilies(physicalDevice);

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueuFamilies = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};

	float const queuePriority = 1.0f;
	for ([[maybe_unused]] uint32_t queueFamily : uniqueQueuFamilies)
	{
		vk::DeviceQueueCreateInfo queueCreateInfo;
		queueCreateInfo.setQueueFamilyIndex(indices.graphicsFamily.value());
		queueCreateInfo.setQueueCount(1);
		queueCreateInfo.setPQueuePriorities(&queuePriority);
		
		queueCreateInfos.push_back(queueCreateInfo);
	}
	
	vk::PhysicalDeviceFeatures deviceFeatures;
	deviceFeatures.setGeometryShader(true);
	deviceFeatures.setSampleRateShading(sampleShadingEnabled);

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

bool RenderEngine::checkDeviceExtensionSupport(vk::PhysicalDevice const & device)
{
	auto const availableExtensions = device.enumerateDeviceExtensionProperties();

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

void RenderEngine::createSwapChain()
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

RenderEngine::SwapChainSupportDetails RenderEngine::querySwapChainSupport(vk::PhysicalDevice const & device)
{
	SwapChainSupportDetails details;

	details.capabilities = device.getSurfaceCapabilitiesKHR(surface.get());
	details.formats = device.getSurfaceFormatsKHR(surface.get());
	details.presentModes = device.getSurfacePresentModesKHR(surface.get());

	return details;
}

vk::SurfaceFormatKHR RenderEngine::chooseSwapSurfaceFormat(std::vector<vk::SurfaceFormatKHR> const& availableFormats)
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

vk::PresentModeKHR RenderEngine::chooseSwapPresentMode(std::vector<vk::PresentModeKHR> const& availablePresentModes)
{
	for (auto const& availablePresentMode : availablePresentModes)
	{
		if (mVSyncEnabled == false)
		{
			if (availablePresentMode == vk::PresentModeKHR::eMailbox)
			{
				return availablePresentMode;
			}
		}
		else
		{
			if (availablePresentMode == vk::PresentModeKHR::eFifo)
			{
				return availablePresentMode;
			}
		}

		

	}

	return vk::PresentModeKHR::eFifo;
}

// there is a fucking min and max macro which prevents using the functions in chooseSwapExtent
#undef max
#undef min

vk::Extent2D RenderEngine::chooseSwapExtent(vk::SurfaceCapabilitiesKHR const & capabilities)
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

void RenderEngine::createImageViews()
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

void RenderEngine::createRenderPass()
{
	vk::AttachmentDescription colorAttachment = {};
	colorAttachment.setFormat(swapChainImageFormat);
	colorAttachment.setSamples(msaaSamples);
	colorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
	colorAttachment.setStoreOp(vk::AttachmentStoreOp::eStore);
	colorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
	colorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
	colorAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
	colorAttachment.setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);

	vk::AttachmentDescription depthAttachment = {};
	depthAttachment.setFormat(findDepthFormat());
	depthAttachment.setSamples(msaaSamples);
	depthAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
	depthAttachment.setStoreOp(vk::AttachmentStoreOp::eDontCare);
	depthAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
	depthAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
	depthAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
	depthAttachment.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

	vk::AttachmentDescription colorAttachmentResolve = {};
	colorAttachmentResolve.setFormat(swapChainImageFormat);
	colorAttachmentResolve.setSamples(vk::SampleCountFlagBits::e1);
	colorAttachmentResolve.setLoadOp(vk::AttachmentLoadOp::eDontCare);
	colorAttachmentResolve.setStoreOp(vk::AttachmentStoreOp::eStore);
	colorAttachmentResolve.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
	colorAttachmentResolve.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
	colorAttachmentResolve.setInitialLayout(vk::ImageLayout::eUndefined);
	colorAttachmentResolve.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

	vk::AttachmentReference colorAttachmentRef;
	colorAttachmentRef.setAttachment(0);
	colorAttachmentRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

	vk::AttachmentReference depthAttachmentRef;
	depthAttachmentRef.setAttachment(1);
	depthAttachmentRef.setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

	vk::AttachmentReference colorAttachmentResolveRef;
	colorAttachmentResolveRef.setAttachment(2);
	colorAttachmentResolveRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

	vk::SubpassDescription subpass = {};
	subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
	subpass.setColorAttachmentCount(1);
	subpass.setPColorAttachments(&colorAttachmentRef);
	subpass.setPDepthStencilAttachment(&depthAttachmentRef);
	subpass.setPResolveAttachments(&colorAttachmentResolveRef);

	std::array<vk::AttachmentDescription, 3> attachements = { colorAttachment, depthAttachment, colorAttachmentResolve };

	vk::RenderPassCreateInfo renderPassInfo;
	renderPassInfo.setAttachmentCount(static_cast<uint32_t>(attachements.size()));
	renderPassInfo.setPAttachments(attachements.data());
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

void RenderEngine::createDescriptorSetLayout(vk::UniqueDescriptorSetLayout & descriptorSetLayout, 
	vk::ShaderStageFlags const shaderStageFlag, bool const useStorageBuffer)
{
	// Uniform Buffer Layout
	vk::DescriptorSetLayoutBinding uboLayoutBinding[2];
	uboLayoutBinding[0].setBinding(0);
	uboLayoutBinding[0].setDescriptorType(vk::DescriptorType::eUniformBuffer);
	uboLayoutBinding[0].setDescriptorCount(1);
	uboLayoutBinding[0].setStageFlags(shaderStageFlag);
	uboLayoutBinding[0].setPImmutableSamplers(nullptr); // Optional

	// Storage Buffer Layout
	uboLayoutBinding[1].setBinding(1);
	uboLayoutBinding[1].setDescriptorType(vk::DescriptorType::eStorageBuffer);
	uboLayoutBinding[1].setDescriptorCount(1);	// if it is an array of objects
	uboLayoutBinding[1].setStageFlags(shaderStageFlag);
	uboLayoutBinding[1].setPImmutableSamplers(nullptr); // Optional
	
	// Create
	size_t const count = useStorageBuffer ? 2 : 1;

	vk::DescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.setBindingCount(static_cast<uint32_t>(count));
	layoutInfo.setPBindings(uboLayoutBinding);

	descriptorSetLayout = device->createDescriptorSetLayoutUnique(layoutInfo);	
}

void RenderEngine::createGraphicsPipeline(vk::UniquePipelineLayout& pipelineLayout,
	vk::UniquePipeline& graphicsPipeline,
	std::vector<char> const& vertShaderCode, std::vector<char> const& fragShaderCode,
	vk::UniqueDescriptorSetLayout const& descriptorSetLayout,
	vk::VertexInputBindingDescription const& bindingDescription,
	std::vector<vk::VertexInputAttributeDescription> const& attributeDescriptions,
	vk::SpecializationInfo specializationVertexInfo, vk::SpecializationInfo specializationGeometryInfo, vk::SpecializationInfo specializationFragmentInfo,
	bool const useTriangles)
{
	createGraphicsPipeline(pipelineLayout, graphicsPipeline, vertShaderCode, std::vector<char>(), fragShaderCode,
		descriptorSetLayout, bindingDescription, attributeDescriptions, 
		specializationVertexInfo, specializationGeometryInfo, specializationFragmentInfo,
		useTriangles);
}


void RenderEngine::createGraphicsPipeline(vk::UniquePipelineLayout& pipelineLayout,
	vk::UniquePipeline& graphicsPipeline,
	std::vector<char> const& vertShaderCode, std::vector<char> const& geomShaderCode, std::vector<char> const& fragShaderCode,
	vk::UniqueDescriptorSetLayout const & descriptorSetLayout,
	vk::VertexInputBindingDescription const& bindingDescription,
	std::vector<vk::VertexInputAttributeDescription>const& attributeDescriptions,
	vk::SpecializationInfo specializationVertexInfo, vk::SpecializationInfo specializationGeometryInfo, vk::SpecializationInfo specializationFragmentInfo,
	bool const useTriangles, bool const alphaBlending)
{
	vk::UniqueShaderModule const vertShaderModule = createShaderModule(vertShaderCode);
	vk::UniqueShaderModule geomShaderModule;
	if (!geomShaderCode.empty())
	{
		geomShaderModule = createShaderModule(geomShaderCode);
	}
	vk::UniqueShaderModule const fragShaderModule = createShaderModule(fragShaderCode);

	vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
	vertShaderStageInfo.setStage(vk::ShaderStageFlagBits::eVertex);
	vertShaderStageInfo.setModule(vertShaderModule.get());
	vertShaderStageInfo.setPName("main");
	if (specializationVertexInfo.mapEntryCount > 0) {
		vertShaderStageInfo.setPSpecializationInfo(&specializationVertexInfo);
	}

	vk::PipelineShaderStageCreateInfo geomShaderStageInfo;
	if (!geomShaderCode.empty())
	{
		geomShaderStageInfo.setStage(vk::ShaderStageFlagBits::eGeometry);
		geomShaderStageInfo.setModule(geomShaderModule.get());
		geomShaderStageInfo.setPName("main");
		if (specializationGeometryInfo.mapEntryCount > 0) {
			geomShaderStageInfo.setPSpecializationInfo(&specializationGeometryInfo);
		}
	}

	vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
	fragShaderStageInfo.setStage(vk::ShaderStageFlagBits::eFragment);
	fragShaderStageInfo.setModule(fragShaderModule.get());
	fragShaderStageInfo.setPName("main");
	if (specializationFragmentInfo.mapEntryCount > 0) {
		fragShaderStageInfo.setPSpecializationInfo(&specializationFragmentInfo);
	}

	size_t shaderStagesSize = 0;
	vk::PipelineShaderStageCreateInfo shaderStages[3];
	if (!geomShaderCode.empty())
	{
		shaderStages[0] = vertShaderStageInfo;
		shaderStages[1] = fragShaderStageInfo;
		shaderStages[2] = geomShaderStageInfo;
		shaderStagesSize = 3;
	}
	else
	{
		shaderStages[0] = vertShaderStageInfo;
		shaderStages[1] = fragShaderStageInfo;
		shaderStagesSize = 2;
	}

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

	vertexInputInfo.setVertexBindingDescriptionCount(1);
	vertexInputInfo.setPVertexBindingDescriptions(&bindingDescription);
	vertexInputInfo.setVertexAttributeDescriptionCount(static_cast<uint32_t>(attributeDescriptions.size()));
	vertexInputInfo.setPVertexAttributeDescriptions(attributeDescriptions.data());

	vk::PipelineInputAssemblyStateCreateInfo inputAssembly;

	if (useTriangles == true && geomShaderCode.empty())
	{
		inputAssembly.setTopology(vk::PrimitiveTopology::eTriangleList);
	}
	else
	{
		//inputAssembly.setTopology(vk::PrimitiveTopology::eTriangleList);
		inputAssembly.setTopology(vk::PrimitiveTopology::ePointList);
	}

	inputAssembly.setPrimitiveRestartEnable(VK_FALSE);

	vk::Viewport viewport;
	viewport.setX(0.0f);
	viewport.setY(0.0f);
	viewport.setWidth(static_cast<float>(swapChainExtent.width));
	viewport.setHeight(static_cast<float>(swapChainExtent.height));
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

	vk::PipelineMultisampleStateCreateInfo multisampling;
	multisampling.setSampleShadingEnable(sampleShadingEnabled);
	multisampling.setRasterizationSamples(msaaSamples);
	multisampling.setMinSampleShading(smapleShadingMinFactor); // Optional
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

	//constexpr bool alphaBlending = false;
	if (!alphaBlending)
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

	pipelineLayout = device->createPipelineLayoutUnique(pipelineLayoutInfo);

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
	pipelineInfo.setRenderPass(renderPass.get());
	pipelineInfo.setSubpass(0);
	//pipelineInfo.setBasePipelineHandle(); // Optional
	pipelineInfo.setBasePipelineIndex(-1); // Optional

	// graphicsPipeline = device->createGraphicsPipelineUnique(nullptr, pipelineInfo);
	graphicsPipeline = device->createGraphicsPipelineUnique(nullptr, pipelineInfo).value;
}


vk::UniqueShaderModule RenderEngine::createShaderModule(std::vector<char> const& code)
{
	vk::ShaderModuleCreateInfo createInfo;
	createInfo.setCodeSize(code.size());
	createInfo.setPCode(reinterpret_cast<uint32_t const*>(code.data()));

	auto shaderModule = device->createShaderModuleUnique(createInfo);

	return shaderModule;
}

void RenderEngine::createFramebuffers()
{
	for (auto const& swapImageView : swapChainImageViews)
	{
		vk::ImageView attachments[] = {
			colorImageView.get(),
			depthImageView.get(),
			swapImageView.get()			
		};

		vk::FramebufferCreateInfo framebufferInfo;
		framebufferInfo.setRenderPass(renderPass.get());
		framebufferInfo.setAttachmentCount(static_cast<uint32_t>(std::size(attachments)));
		framebufferInfo.setPAttachments(attachments);
		framebufferInfo.setWidth(swapChainExtent.width);
		framebufferInfo.setHeight(swapChainExtent.height);
		framebufferInfo.setLayers(1);

		swapChainFramebuffers.push_back(device->createFramebufferUnique(framebufferInfo));
	}
}

void RenderEngine::createCommandPool()
{
	QueueFamilyIndices const queueFamilyIndices = findQueueFamilies(physicalDevice);

	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.setQueueFamilyIndex(queueFamilyIndices.graphicsFamily.value());
	//poolInfo.setFlags( ); // Optional

	commandPool = device->createCommandPoolUnique(poolInfo);
}

void RenderEngine::createColorResources()
{
	vk::Format const colorFormat = swapChainImageFormat;

	createImage(swapChainExtent.width, swapChainExtent.height, msaaSamples, colorFormat,
		vk::ImageTiling::eOptimal, 
		vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
		vk::MemoryPropertyFlagBits::eDeviceLocal, colorImage, colorImageMemory);

	colorImageView = createImageView(colorImage.get(), colorFormat, vk::ImageAspectFlagBits::eColor);
}

void RenderEngine::createDepthResources()
{
	vk::Format const depthFormat = findDepthFormat();

	createImage(swapChainExtent.width, swapChainExtent.height, msaaSamples,
		depthFormat, 
		vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment,
		vk::MemoryPropertyFlagBits::eDeviceLocal, 
		depthImage, depthImageMemory);

	depthImageView = createImageView(depthImage.get(), depthFormat, vk::ImageAspectFlagBits::eDepth);

}

vk::Format RenderEngine::findDepthFormat()
{
	return findSupportedFormat(
		{vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
		vk::ImageTiling::eOptimal,
		vk::FormatFeatureFlagBits::eDepthStencilAttachment
	);
}

vk::Format RenderEngine::findSupportedFormat(std::vector<vk::Format> const& candidates, 
	vk::ImageTiling tiling, vk::FormatFeatureFlags features)
{
	for (vk::Format format : candidates) {
		vk::FormatProperties props;
		physicalDevice.getFormatProperties(format, &props);

		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
	return vk::Format();
}

bool RenderEngine::hasStencilComponent(vk::Format format)
{
	return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
}

void RenderEngine::createImage(uint32_t const width, uint32_t const height,
	vk::SampleCountFlagBits const numSample,
	vk::Format const & format, vk::ImageTiling const & tiling, 
	vk::ImageUsageFlags const & usage, vk::MemoryPropertyFlags const & properties, 
	vk::UniqueImage& image, vk::UniqueDeviceMemory& imageMemory)
{
	vk::ImageCreateInfo imageInfo;
	imageInfo.setImageType(vk::ImageType::e2D);
	imageInfo.extent.setWidth(width);
	imageInfo.extent.setHeight(height);
	imageInfo.extent.setDepth(1);
	imageInfo.setMipLevels(1);
	imageInfo.setArrayLayers(1);
	imageInfo.setFormat(format);
	imageInfo.setTiling(tiling);
	imageInfo.setInitialLayout(vk::ImageLayout::eUndefined);
	imageInfo.setUsage(usage);
	imageInfo.setSamples(numSample);
	imageInfo.setSharingMode(vk::SharingMode::eExclusive);

	image = device->createImageUnique(imageInfo);

	vk::MemoryRequirements const memRequirements = device->getImageMemoryRequirements(image.get());;
	
	vk::MemoryAllocateInfo allocInfo;
	allocInfo.setAllocationSize(memRequirements.size);
	allocInfo.setMemoryTypeIndex(findMemoryType(memRequirements.memoryTypeBits, properties));

	imageMemory = device->allocateMemoryUnique(allocInfo);

	device->bindImageMemory(image.get(), imageMemory.get(), 0);
}

vk::UniqueImageView RenderEngine::createImageView(vk::Image const& image, 
	vk::Format const& format, vk::ImageAspectFlags const& aspectFlags)
{
	vk::ImageViewCreateInfo viewInfo;
	viewInfo.setImage(image);
	viewInfo.setViewType(vk::ImageViewType::e2D);
	viewInfo.setFormat(format);
	viewInfo.subresourceRange.setAspectMask(aspectFlags);
	viewInfo.subresourceRange.setBaseMipLevel(0);
	viewInfo.subresourceRange.setLevelCount(1);
	viewInfo.subresourceRange.setBaseArrayLayer(0);
	viewInfo.subresourceRange.setLayerCount(1);

	return device->createImageViewUnique(viewInfo);
}


uint32_t RenderEngine::findMemoryType(uint32_t const typeFilter, vk::MemoryPropertyFlags const& properties)
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

void RenderEngine::createBuffer(vk::DeviceSize const size, 
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

void RenderEngine::copyBuffer(vk::Buffer const& srcBuffer, vk::Buffer& dstBuffer, vk::DeviceSize size)
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


void RenderEngine::createBuffers(std::vector<vk::UniqueDeviceMemory>& bufferMemory,
	std::vector<vk::UniqueBuffer>& buffers, size_t const bufferSize, vk::BufferUsageFlagBits usageFlags)
{
	assert(!swapChainImages.empty());

	vk::DeviceSize const deviceBufferSize = bufferSize;

	buffers.resize(swapChainImages.size());
	bufferMemory.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); ++i)
	{
		// https://zeux.io/2020/02/27/writing-an-efficient-vulkan-renderer/
		createBuffer(deviceBufferSize, usageFlags,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eDeviceLocal,
			buffers[i], bufferMemory[i]
		);
	}
}

void RenderEngine::createVertexBuffers(std::vector<vk::UniqueDeviceMemory>& vertexBufferMemory,
	std::vector<vk::UniqueBuffer>& vertexBuffers, size_t const vertexBufferSize)
{
	createBuffers(vertexBufferMemory, vertexBuffers, vertexBufferSize, vk::BufferUsageFlagBits::eVertexBuffer);
}

void RenderEngine::createStorageBuffers(std::vector<vk::UniqueDeviceMemory>& storageBufferMemory, 
	std::vector<vk::UniqueBuffer>& storageBuffers, size_t const storageBufferSize)
{
	createBuffers(storageBufferMemory, storageBuffers, storageBufferSize, vk::BufferUsageFlagBits::eStorageBuffer);
}

void RenderEngine::createUniformBuffers(std::vector<vk::UniqueDeviceMemory>& uniformBuffersMemory, 
	std::vector<vk::UniqueBuffer>& uniformBuffers, size_t const UniformBufferObjectSize)
{
	vk::DeviceSize const bufferSize = UniformBufferObjectSize;

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


void RenderEngine::createDescriptorPool(vk::UniqueDescriptorPool & descriptorPool)
{
	std::array<vk::DescriptorPoolSize, 2> poolSize;
	poolSize[0].setType(vk::DescriptorType::eUniformBuffer);
	poolSize[0].setDescriptorCount(static_cast<uint32_t>(swapChainImages.size()));
	poolSize[1].setType(vk::DescriptorType::eStorageBuffer);
	poolSize[1].setDescriptorCount(static_cast<uint32_t>(swapChainImages.size()));

	vk::DescriptorPoolCreateInfo poolInfo;
	poolInfo.setPoolSizeCount(static_cast<uint32_t>(poolSize.size()));
	poolInfo.setPPoolSizes(poolSize.data());
	poolInfo.setMaxSets(static_cast<uint32_t>(swapChainImages.size()));

	descriptorPool = device->createDescriptorPoolUnique(poolInfo);
}

void RenderEngine::createDescriptorSets(vk::UniqueDescriptorPool& descriptorPool, 
	std::vector<vk::DescriptorSet>& descriptorSets,
	vk::UniqueDescriptorSetLayout const& descriptorSetLayout,
	std::vector<vk::UniqueBuffer> const& uniformBuffers, size_t const UniformBufferObjectSize,
	std::vector<vk::UniqueBuffer> const& storageBuffers, size_t const StorageBufferObjectSize)
{
	assert(!uniformBuffers.empty());

	std::vector<vk::DescriptorSetLayout> const layouts(swapChainImages.size(), descriptorSetLayout.get());

	vk::DescriptorSetAllocateInfo allocInfo;
	allocInfo.setDescriptorPool(descriptorPool.get());
	allocInfo.setDescriptorSetCount(static_cast<uint32_t>(swapChainImages.size()));
	allocInfo.setPSetLayouts(layouts.data());

	descriptorSets = device->allocateDescriptorSets(allocInfo);

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		// Uniform Buffer
		vk::DescriptorBufferInfo uniformBufferInfo;
		uniformBufferInfo.setBuffer(uniformBuffers[i].get());
		uniformBufferInfo.setOffset(0);
		uniformBufferInfo.setRange(UniformBufferObjectSize);

		// Storage Buffer
		vk::DescriptorBufferInfo storageBufferInfo = {};
		if (StorageBufferObjectSize > 0)
		{
			storageBufferInfo.setBuffer(storageBuffers[i].get());
			storageBufferInfo.setOffset(0);
			storageBufferInfo.setRange(StorageBufferObjectSize);
		}
		
		vk::WriteDescriptorSet descriptorWritesUbo = {};
		descriptorWritesUbo.setDstSet(descriptorSets[i]);
		descriptorWritesUbo.setDstBinding(0);
		descriptorWritesUbo.setDstArrayElement(0);
		descriptorWritesUbo.setDescriptorType(vk::DescriptorType::eUniformBuffer);
		descriptorWritesUbo.setDescriptorCount(1);
		descriptorWritesUbo.setPBufferInfo(&uniformBufferInfo);
		descriptorWritesUbo.setPImageInfo(nullptr); // Optional
		descriptorWritesUbo.setPTexelBufferView(nullptr); // Optional

		vk::WriteDescriptorSet descriptorWritesSbo = {};
		descriptorWritesSbo.setDstSet(descriptorSets[i]);
		descriptorWritesSbo.setDstBinding(1);
		descriptorWritesSbo.setDstArrayElement(0);
		descriptorWritesSbo.setDescriptorType(vk::DescriptorType::eStorageBuffer);
		descriptorWritesSbo.setDescriptorCount(1);
		descriptorWritesSbo.setPBufferInfo(&storageBufferInfo);
		descriptorWritesSbo.setPImageInfo(nullptr); // Optional
		descriptorWritesSbo.setPTexelBufferView(nullptr); // Optional

		if (StorageBufferObjectSize > 0)
		{	
			std::array<vk::WriteDescriptorSet, 2> descriptorWrites = { descriptorWritesUbo , descriptorWritesSbo };
			device->updateDescriptorSets(descriptorWrites, nullptr);
		}
		else
		{	// Storage Buffer is not used
			std::array<vk::WriteDescriptorSet, 1> descriptorWrites = { descriptorWritesUbo };
			device->updateDescriptorSets(descriptorWrites, nullptr);
		}
	}
}

void RenderEngine::createCommandBuffersBegin()
{
	assert(device);
	assert(commandPool);
	assert(!swapChainFramebuffers.empty());

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

		std::array<vk::ClearValue, 2> clearValues = {};
		clearValues[0].setColor(std::array<float, 4>{ 0.0f, 0.0f, 0.0f, 1.0f });
		clearValues[1].setDepthStencil({ 1.0f, 0 });

		renderPassInfo.setClearValueCount(static_cast<uint32_t>(clearValues.size()));
		renderPassInfo.setPClearValues(clearValues.data());

		commandBuffers[i]->beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
	}
}

void RenderEngine::createCommandBuffersEnd()
{
	assert(!commandBuffers.empty());

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		commandBuffers[i]->endRenderPass();

		commandBuffers[i]->end();
	}
}

void RenderEngine::recordCommands(vk::UniquePipelineLayout const& pipelineLayout, vk::UniquePipeline const& graphicsPipeline, vk::UniqueBuffer const& vertexBuffer, std::vector<vk::DescriptorSet> const& descriptorSets, size_t const verticesCount)
{
	assert(pipelineLayout);
	assert(graphicsPipeline);
	assert(!vertexBuffer);
	assert(!descriptorSets.empty());
	assert(device);
	assert(commandPool);
	assert(!swapChainFramebuffers.empty());

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		commandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline.get());

		// #pragma warning( push )
		// #pragma warning( disable : 4297 )
		commandBuffers[i]->bindVertexBuffers(0, vertexBuffer.get(), vk::DeviceSize());
		// #pragma warning( pop ) 

		commandBuffers[i]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSets[i], nullptr);

		commandBuffers[i]->draw(static_cast<uint32_t>(verticesCount), 1, 0, 0);
	}
}

void RenderEngine::recordCommands(vk::UniquePipelineLayout const& pipelineLayout, 
	vk::UniquePipeline const& graphicsPipeline,
	std::vector<vk::UniqueBuffer> const& vertexBuffers,
	std::vector<vk::DescriptorSet> const& descriptorSets,
	size_t const verticesCount, 
	char const * name)
{
	assert(pipelineLayout);
	assert(graphicsPipeline);
	assert(!vertexBuffers.empty());
	assert(!descriptorSets.empty());
	assert(device);
	assert(commandPool);
	assert(!swapChainFramebuffers.empty());


	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		// static size_t iMarker = 0;
		vk::DebugUtilsLabelEXT debugMarkerInfo = {};
		debugMarkerInfo.setPLabelName(name);

		vkCmdBeginDebugUtilsLabelEXT(instance.get(), commandBuffers[i].get(), debugMarkerInfo);

		commandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline.get());

		commandBuffers[i]->bindVertexBuffers(0, vertexBuffers[i].get(), vk::DeviceSize());

		commandBuffers[i]->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.get(), 0, descriptorSets[i], nullptr);

		commandBuffers[i]->draw(static_cast<uint32_t>(verticesCount), 1, 0, 0);

		vkCmdEndDebugUtilsLabelEXT(instance.get(), commandBuffers[i].get());
	}
}




void RenderEngine::createSyncObjects()
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

void RenderEngine::mainLoop()
{
	while (!glfwWindowShouldClose(window.get())) {
		glfwPollEvents();
		//drawFrame();

		drawFrameStart();
		startOfNextFrame();

		assert(currentImageResultValue.result == vk::Result::eSuccess);
		uint32_t const imageIndex = currentImageResultValue.value;

		for (auto const& elem : mObjs)
		{
			elem.get().draw(*this, imageIndex);
		}
		drawFrameEnd();

		mFPS.showFPS(window.get());
	}

	device->waitIdle();
	
}

void RenderEngine::drawFrameStart()
{
	[[maybe_unused]] auto res = device->waitForFences(inFlightFences[currentFrame].get(), VK_TRUE, UINT64_MAX);

	//vk::ResultValue<uint32_t> res(vk::Result::eSuccess, 0);
	try {
		currentImageResultValue = device->acquireNextImageKHR(swapChain.get(), UINT64_MAX, imageAvailableSemaphores[currentFrame].get(), nullptr);
		if (currentImageResultValue.result == vk::Result::eErrorOutOfDateKHR)	// window was resized, swap chain is now incompatible
		{
			recreateSwapChain();
			return;
		}
		else if (currentImageResultValue.result != vk::Result::eSuccess && currentImageResultValue.result != vk::Result::eSuboptimalKHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}
	}
	catch (vk::OutOfDateKHRError const&)
	{
		recreateSwapChain();
		return;
	}
}

void RenderEngine::drawFrameEnd()
{
	assert(currentImageResultValue.result == vk::Result::eSuccess);
	uint32_t const imageIndex = currentImageResultValue.value;

	graphicsQueue.waitIdle();

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
		else if (currentImageResultValue.result != vk::Result::eSuccess) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}
	}
	catch (vk::OutOfDateKHRError const&)
	{
		framebufferResized = false;
		recreateSwapChain();
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}


void RenderEngine::cleanup()
{

}

void RenderEngine::recreateSwapChain()
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
	//createGraphicsPipeline();
	createColorResources();
	createDepthResources();
	createFramebuffers();
	//createUniformBuffers();
	//createDescriptorPool();
	//createDescriptorSets();
	createCommandBuffersBegin();

	for (auto& elem : mObjs)
	{
		elem.get().setup(*this);
	}

	createCommandBuffersEnd();
	
}

void RenderEngine::cleanupSwapChain()
{
	for (auto& elem : mObjs)
	{
		elem.get().cleanup(*this);
	}

	swapChainFramebuffers.clear();
	//descriptorSets.clear();
	//descriptorPool.reset();	
	commandBuffers.clear();
	//uniformBuffers.clear();
	//uniformBuffersMemory.clear();
	colorImageView.reset();
	colorImage.reset();
	colorImageMemory.reset();
	depthImageView.reset();
	depthImage.reset();
	depthImageMemory.reset();
	//graphicsPipeline.reset();
	//pipelineLayout.reset();
	renderPass.reset();
	swapChainImageViews.clear();
	swapChain.reset();
}

VKAPI_ATTR VkBool32 VKAPI_CALL RenderEngine::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		// Message is important enough to show
	}

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void RenderEngine::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<RenderEngine*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

void RenderEngine::keyInputCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	auto app = reinterpret_cast<RenderEngine*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) 
	{
		app->keyPressed(GLFWKeyToKey(key));
	}
	if (action == GLFW_RELEASE) 
	{
		app->keyReleased(GLFWKeyToKey(key));
	}
}

void RenderEngine::setVSync(bool const vsyncEnabled)
{
	mVSyncEnabled = true;
	recreateSwapChain();
}

void RenderEngine::setWindowSize(size_t const widht, size_t const height, bool const fullscreenEnabled)
{
	assert(window);

	mWidth = widht;
	mHeight = height;
	mFullScreenEnabled = true;

	if (fullscreenEnabled == false)
	{
		glfwSetWindowMonitor(window.get(), nullptr, 0, 30, static_cast<int>(widht), static_cast<int>(height), GLFW_DONT_CARE);
	}
	else
	{
		glfwSetWindowMonitor(window.get(), glfwGetPrimaryMonitor(), 0, 30, static_cast<int>(widht), static_cast<int>(height), GLFW_DONT_CARE);
	}
}





