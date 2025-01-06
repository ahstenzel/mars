#include "mars/renderer_vk.h"
#include "mars/game.h"
#include "mars/shader.h"

#define MARS_PHYSICAL_DEVICE(w) ((w)->_physicalDevices[(w)->_physicalDeviceIdx])
#define MARS_VK_RENDERER ((RendererVulkan*)MARS_DISPLAY->_renderer)

static void _RendererVKFramebufferResizeCallback(GLFWwindow* _window, int _width, int _height) {
	RendererVulkan* renderer = glfwGetWindowUserPointer(_window);
	renderer->_framebufferResized = true;
}

RendererVulkan* _RendererVKCreate(GLFWwindow* _window) {
	MARS_RETURN_CLEAR;
	RendererVulkan* renderer = NULL;
	char* vertexShaderCode = NULL;
	char* fragmentShaderCode = NULL;

	// Allocate renderer
	renderer = MARS_CALLOC(1, sizeof(*renderer));
	if (!renderer) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate renderer structure!");
		goto renderer_vk_create_fail;
	}
	renderer->_framebufferResized = false;

	// Register callbacks
	glfwSetWindowUserPointer(_window, &renderer);
	glfwSetFramebufferSizeCallback(_window, _RendererVKFramebufferResizeCallback);

	// Initialize Vulkan
	MARS_DEBUG_LOG("Initializing Vulkan");
	renderer->_instance = _RendererVKCreateInstance();
	if (MARS_RETURN_CODE != MARS_RETURN_CODE_OK) {
		MARS_DEBUG_WARN("Failed to initalize render backend!");
		goto renderer_vk_create_fail;
	}

	// Create physical device
	MARS_DEBUG_LOG("Creating physical device");
	renderer->_numPhysicalDevices = _RendererVKGetPhysicalDeviceNumber(&renderer->_instance);
	renderer->_physicalDevices = _RendererVKGetPhysicalDevices(&renderer->_instance, renderer->_numPhysicalDevices);
	renderer->_physicalDeviceIdx = _RendererVKGetBestPhysicalDeviceIndex(renderer->_physicalDevices, renderer->_numPhysicalDevices);

	// Create render queue
	MARS_DEBUG_LOG("Creating render queue");
	uint32_t numQueueFamily = _RendererVKGetQueueFamilyNumber(&MARS_PHYSICAL_DEVICE(renderer));
	VkQueueFamilyProperties* queueFamilyProperties = _RendererVKGetQueueFamilyProperties(&MARS_PHYSICAL_DEVICE(renderer), numQueueFamily);
	renderer->_device = _RendererVKCreateDevice(&MARS_PHYSICAL_DEVICE(renderer), numQueueFamily, queueFamilyProperties);
	uint32_t bestGraphicsQueueFamilyIndex = _RendererVKGetBestGraphicsQueueFamilyIndex(queueFamilyProperties, numQueueFamily);
	renderer->_graphicsQueueMode = _RendererVKGetGraphicsQueueMode(queueFamilyProperties, bestGraphicsQueueFamilyIndex);
	renderer->_drawingQueue = _RendererVKGetDrawingQueue(&renderer->_device, bestGraphicsQueueFamilyIndex);
	renderer->_presentingQueue = _RendererVKGetPresentingQueue(&renderer->_device, bestGraphicsQueueFamilyIndex, renderer->_graphicsQueueMode);
	_RendererVKDestroyQueueFamilyProperties(&queueFamilyProperties);

	// Create surface
	MARS_DEBUG_LOG("Create surface");
	renderer->_surface = _RendererVKCreateSurface(_window, &renderer->_instance);
	VkBool32 surfaceSupported = _RendererVKGetSurfaceSupport(&renderer->_surface, &MARS_PHYSICAL_DEVICE(renderer), bestGraphicsQueueFamilyIndex);
	if (!surfaceSupported) {
		MARS_DEBUG_WARN("Failed to create swapchain!");
		goto renderer_vk_create_fail;
	}
	
	// Create swapchain
	MARS_DEBUG_LOG("Creating swapchain");
	VkSurfaceCapabilitiesKHR surfaceCapabilities = _RendererVKGetSurfaceCapabilities(&renderer->_surface, &MARS_PHYSICAL_DEVICE(renderer));
	VkSurfaceFormatKHR bestSurfaceFormat = _RendererVKGetBestSurfaceFormat(&renderer->_surface, &MARS_PHYSICAL_DEVICE(renderer));
	VkPresentModeKHR bestPresentMode = _RendererVKGetBestPresentMode(&renderer->_surface, &MARS_PHYSICAL_DEVICE(renderer));
	VkExtent2D bestSwapchainExtent = _RendererVKGetBestSwapchainExtent(&surfaceCapabilities, _window);
	uint32_t imageArrayLayers = 1;
	renderer->_swapchain = _RendererVKCreateSwapchain(&renderer->_device, &renderer->_surface, &surfaceCapabilities, &bestSurfaceFormat, &bestSwapchainExtent, &bestPresentMode, imageArrayLayers, renderer->_graphicsQueueMode);
	renderer->_numSwapchainImages = _RendererVKGetSwapchainImageNumber(&renderer->_device, &renderer->_swapchain);
	renderer->_swapchainImages = _RendererVKGetSwapchainImages(&renderer->_device, &renderer->_swapchain, renderer->_numSwapchainImages);
	renderer->_swapchainImageViews = _RendererVKCreateImageViews(&renderer->_device, &renderer->_swapchainImages, &bestSurfaceFormat, renderer->_numSwapchainImages, imageArrayLayers);

	// Create framebuffers
	MARS_DEBUG_LOG("Creating framebuffer");
	renderer->_renderPass = _RendererVKCreateRenderPass(&renderer->_device, &bestSurfaceFormat);
	renderer->_framebuffers = _RendererVKCreateFramebuffers(&renderer->_device, &renderer->_renderPass, &bestSwapchainExtent, &renderer->_swapchainImageViews, renderer->_numSwapchainImages);

	// Create shader pipelines
	MARS_DEBUG_LOG("Creating default vertex shader module");
	size_t vertexShaderSize = base64Decode(_SHADER_BIN_DEFAULT_VERT_SPV, NULL, 0);
	vertexShaderCode = MARS_MALLOC(vertexShaderSize);
	if (!vertexShaderCode) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate default vertex shader code buffer!");
		goto renderer_vk_create_fail;
	}
	if (base64Decode(_SHADER_BIN_DEFAULT_VERT_SPV, vertexShaderCode, vertexShaderSize) != vertexShaderSize) {
		MARS_DEBUG_WARN("Failed to decode vertex shader code!");
		MARS_RETURN_SET(MARS_RETURN_CODE_GENERIC_ERROR);
		goto renderer_vk_create_fail;
	}
	VkShaderModule vertexShaderModule = _RendererVKCreateShaderModule(&renderer->_device, vertexShaderCode, (uint32_t)vertexShaderSize);
	MARS_FREE(vertexShaderCode);
	vertexShaderCode = NULL;

	MARS_DEBUG_LOG("Creating default fragment shader module");
	size_t fragmentShaderSize = base64Decode(_SHADER_BIN_DEFAULT_FRAG_SPV, NULL, 0);
	fragmentShaderCode = MARS_MALLOC(fragmentShaderSize);
	if (!fragmentShaderCode) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate default fragment shader code buffer!");
		goto renderer_vk_create_fail;
	}
	if (base64Decode(_SHADER_BIN_DEFAULT_FRAG_SPV, fragmentShaderCode, fragmentShaderSize) != fragmentShaderSize) {
		MARS_DEBUG_WARN("Failed to decode fragment shader code!");
		MARS_RETURN_SET(MARS_RETURN_CODE_GENERIC_ERROR);
		goto renderer_vk_create_fail;
	}
	VkShaderModule fragmentShaderModule = _RendererVKCreateShaderModule(&renderer->_device, fragmentShaderCode, (uint32_t)fragmentShaderSize);
	MARS_FREE(fragmentShaderCode);
	fragmentShaderCode = NULL;

	MARS_DEBUG_LOG("Creating default shader pipeline");
	renderer->_pipelineLayout = _RendererVKCreatePipelineLayout(&renderer->_device);
	renderer->_graphicsPipeline = _RendererVKCreateGraphicsPipeline(&renderer->_device, &renderer->_pipelineLayout, &vertexShaderModule, &fragmentShaderModule, &renderer->_renderPass, &bestSwapchainExtent);
	_RendererVKDestroyShaderModule(&renderer->_device, &fragmentShaderModule);
	_RendererVKDestroyShaderCode(&fragmentShaderCode);
	_RendererVKDestroyShaderModule(&renderer->_device, &vertexShaderModule);
	_RendererVKDestroyShaderCode(&vertexShaderCode);

	// Create command buffers
	MARS_DEBUG_LOG("Creating command buffers");
	renderer->_commandPool = _RendererVKCreateCommandPool(&renderer->_device, bestGraphicsQueueFamilyIndex);
	renderer->_commandBuffers = _RendererVKCreateCommandBuffers(&renderer->_device, &renderer->_commandPool, renderer->_numSwapchainImages);
	_RendererVKRecordCommandBuffers(&renderer->_commandBuffers, &renderer->_renderPass, &renderer->_framebuffers, &bestSwapchainExtent, &renderer->_graphicsPipeline, renderer->_numSwapchainImages);

	// Create semaphores
	MARS_DEBUG_LOG("Creating semaphores");
	renderer->_maxFrames = 2;
	renderer->_waitSemaphores = _RendererVKCreateSemaphores(&renderer->_device, renderer->_maxFrames);
	renderer->_signalSemaphores = _RendererVKCreateSemaphores(&renderer->_device, renderer->_maxFrames);
	renderer->_frontFences = _RendererVKCreateFences(&renderer->_device, renderer->_maxFrames);
	renderer->_backFences = _RendererVKCreateEmptyFences(renderer->_numSwapchainImages);

	return renderer;
renderer_vk_create_fail:
	_RendererVKDestroy(renderer);
	if (vertexShaderCode) { free(vertexShaderCode); }
	if (fragmentShaderCode) { free(fragmentShaderCode); }
	return NULL;
}

void _RendererVKDestroy(RendererVulkan* _renderer) {
	if (_renderer) {
		vkDeviceWaitIdle(_renderer->_device);
		_RendererVKDestroyEmptyFences(&_renderer->_backFences);
		_RendererVKDestroyFences(&_renderer->_device, &_renderer->_frontFences, _renderer->_maxFrames);
		_RendererVKDestroySemaphores(&_renderer->_device, &_renderer->_signalSemaphores, _renderer->_maxFrames);
		_RendererVKDestroySemaphores(&_renderer->_device, &_renderer->_waitSemaphores, _renderer->_maxFrames);
		_RendererVKDestroyCommandBuffers(&_renderer->_device, &_renderer->_commandBuffers, &_renderer->_commandPool, _renderer->_numSwapchainImages);
		_RendererVKDestroyCommandPool(&_renderer->_device, &_renderer->_commandPool);
		_RendererVKDestroyGraphicsPipeline(&_renderer->_device, &_renderer->_graphicsPipeline);
		_RendererVKDestroyPipelineLayout(&_renderer->_device, &_renderer->_pipelineLayout);
		_RendererVKDestroyFramebuffers(&_renderer->_device, &_renderer->_framebuffers, _renderer->_numSwapchainImages);
		_RendererVKDestroyRenderPass(&_renderer->_device, &_renderer->_renderPass);
		_RendererVKDestroyImageViews(&_renderer->_device, &_renderer->_swapchainImageViews, _renderer->_numSwapchainImages);
		_RendererVKDestroySwapchainImages(&_renderer->_swapchainImages);
		_RendererVKDestroySwapchain(&_renderer->_device, &_renderer->_swapchain);
		_RendererVKDestroySurface(&_renderer->_surface, &_renderer->_instance);
		_RendererVKDestroyDevice(&_renderer->_device);
		_RendererVKDestroyPhysicalDevices(&_renderer->_physicalDevices);
		_RendererVKDestroyInstance(&_renderer->_instance);
	}
}

void _RendererVKUpdate(RendererVulkan* _renderer) {
	// Wait for the last frame to finish
	static uint32_t currentFrame = 0;
	vkWaitForFences(_renderer->_device, 1, &_renderer->_frontFences[currentFrame], VK_TRUE, UINT64_MAX);

	// Get the next image from the swap chain
	uint32_t imageIndex = 0;
	VkResult res = vkAcquireNextImageKHR(_renderer->_device, _renderer->_swapchain, UINT64_MAX, _renderer->_waitSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
	if (res == VK_ERROR_OUT_OF_DATE_KHR) {
		_RendererVKRecreateSwapchain(_renderer, MARS_WINDOW);
		return;
	}
	else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
		MARS_ABORT(MARS_ERROR_CODE_RENDERER, "Failed to acquire next swap chain image!");
		goto renderer_vk_update_fail;
	}
	if (_renderer->_backFences[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(_renderer->_device, 1, &_renderer->_backFences[imageIndex], VK_TRUE, UINT64_MAX);
	}
	_renderer->_backFences[imageIndex] = _renderer->_frontFences[currentFrame];

	// Submit commands to draw queue
	VkPipelineStageFlags pipelineStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,
		VK_NULL_HANDLE,
		1,
		&_renderer->_waitSemaphores[currentFrame],
		&pipelineStage,
		1,
		&_renderer->_commandBuffers[imageIndex],
		1,
		&_renderer->_signalSemaphores[currentFrame]
	};
	vkResetFences(_renderer->_device, 1, &_renderer->_frontFences[currentFrame]);
	res = vkQueueSubmit(_renderer->_drawingQueue, 1, &submitInfo, _renderer->_frontFences[currentFrame]);
	if (res != VK_SUCCESS) {
		MARS_ABORT(MARS_ERROR_CODE_RENDERER, "Failed to submit draw command buffer!");
		goto renderer_vk_update_fail;
	}

	// Present next image
	VkPresentInfoKHR presentInfo = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		VK_NULL_HANDLE,
		1,
		&_renderer->_signalSemaphores[currentFrame],
		1,
		&_renderer->_swapchain,
		&imageIndex,
		VK_NULL_HANDLE
	};
	res = vkQueuePresentKHR(_renderer->_presentingQueue, &presentInfo);
	if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR || _renderer->_framebufferResized) {
		_renderer->_framebufferResized = false;
		_RendererVKRecreateSwapchain(_renderer, MARS_WINDOW);
	}
	else if (res != VK_SUCCESS) {
		MARS_ABORT(MARS_ERROR_CODE_RENDERER, "Failed to present swap chain image!");
		goto renderer_vk_update_fail;
	}
	currentFrame = (currentFrame + 1) % _renderer->_maxFrames;

	return;
renderer_vk_update_fail:
	_DestroyDisplay(MARS_DISPLAY);
}

void _RendererVKRecreateSwapchain(RendererVulkan* _renderer, GLFWwindow* _window) {
	// Halt on minimization
	int width = 0, height = 0;
	glfwGetFramebufferSize(_window, &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(_window, &width, &height);
		glfwWaitEvents();
	}

	// Query vulkan API for new swap chain properties
	vkDeviceWaitIdle(_renderer->_device);
	_RendererVKCleanupSwapchain(_renderer);
	VkSurfaceCapabilitiesKHR surfaceCapabilities = _RendererVKGetSurfaceCapabilities(&_renderer->_surface, &MARS_PHYSICAL_DEVICE(_renderer));
	VkSurfaceFormatKHR bestSurfaceFormat = _RendererVKGetBestSurfaceFormat(&_renderer->_surface, &MARS_PHYSICAL_DEVICE(_renderer));
	VkPresentModeKHR bestPresentMode = _RendererVKGetBestPresentMode(&_renderer->_surface, &MARS_PHYSICAL_DEVICE(_renderer));
	VkExtent2D bestSwapchainExtent = _RendererVKGetBestSwapchainExtent(&surfaceCapabilities, _window);
	uint32_t imageArrayLayers = 1;

	// Recreate assets
	_renderer->_swapchain = _RendererVKCreateSwapchain(&_renderer->_device, &_renderer->_surface, &surfaceCapabilities, &bestSurfaceFormat, &bestSwapchainExtent, &bestPresentMode, imageArrayLayers, _renderer->_graphicsQueueMode);
	_renderer->_numSwapchainImages = _RendererVKGetSwapchainImageNumber(&_renderer->_device, &_renderer->_swapchain);
	_renderer->_swapchainImages = _RendererVKGetSwapchainImages(&_renderer->_device, &_renderer->_swapchain, _renderer->_numSwapchainImages);
	_renderer->_swapchainImageViews = _RendererVKCreateImageViews(&_renderer->_device, &_renderer->_swapchainImages, &bestSurfaceFormat, _renderer->_numSwapchainImages, imageArrayLayers);
	_renderer->_framebuffers = _RendererVKCreateFramebuffers(&_renderer->_device, &_renderer->_renderPass, &bestSwapchainExtent, &_renderer->_swapchainImageViews, _renderer->_numSwapchainImages);
	_RendererVKRecordCommandBuffers(&_renderer->_commandBuffers, &_renderer->_renderPass, &_renderer->_framebuffers, &bestSwapchainExtent, &_renderer->_graphicsPipeline, _renderer->_numSwapchainImages);
}

void _RendererVKCleanupSwapchain(RendererVulkan* _renderer) {
	_RendererVKDestroyFramebuffers(&_renderer->_device, &_renderer->_framebuffers, _renderer->_numSwapchainImages);
	_RendererVKDestroyImageViews(&_renderer->_device, &_renderer->_swapchainImageViews, _renderer->_numSwapchainImages);
	_RendererVKDestroySwapchainImages(&_renderer->_swapchainImages);
	_RendererVKDestroySwapchain(&_renderer->_device, &_renderer->_swapchain);
}

VkInstance _RendererVKCreateInstance() {
	MARS_RETURN_CLEAR;
	char** extensions = NULL;

	// Get GLFW extensionss
	uint32_t numExtensions = 0;
	const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&numExtensions);
	if (!glfw_extensions) {
		MARS_ABORT(MARS_ERROR_CODE_RENDERER, "Failed to get required extensions!");
		goto renderer_vk_create_instance_fail;
	}
	extensions = MARS_MALLOC(numExtensions * sizeof(*extensions));
	if (!extensions) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate extension name buffer!");
		goto renderer_vk_create_instance_fail;
	}
	for(size_t i = 0; i < numExtensions; ++i) {
		extensions[i] = _mars_strdup(glfw_extensions[i]);
		if (!extensions[i]) {
			MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate extension name string (%d)!", (int)i);
			goto renderer_vk_create_instance_fail;
		}
	}

	// Get optional extensions
	#if defined(MARS_DEBUG)
	const char** new_extensions = MARS_REALLOC(extensions, (numExtensions + 1) * sizeof(*extensions));
	new_extensions[numExtensions] = _mars_strdup(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	if (!new_extensions[numExtensions]) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate debug extension name string!");
		goto renderer_vk_create_instance_fail;
	}
	numExtensions++;
	extensions = new_extensions;
	#endif

	MARS_DEBUG_LOG("Vulkan extensions:");
	for(size_t s = 0; s < numExtensions; ++s) {
		MARS_DEBUG_LOG(" (%s)", extensions[s]);
	}

	// Create instance
	VkApplicationInfo applicationInfo = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		VK_NULL_HANDLE,
		VK_NULL_HANDLE,
		0,
		VK_NULL_HANDLE,
		0,
		VK_API_VERSION_1_0
	};
	const char layerList[][VK_MAX_EXTENSION_NAME_SIZE] = {
		"VK_LAYER_KHRONOS_validation"
	};
	const char* layers[] = {
		layerList[0]
	};
	VkInstanceCreateInfo instanceCreateInfo = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		&applicationInfo,
		1,
		layers,
		numExtensions,
		extensions
	};
	VkInstance instance;
	VkResult res;
	if ((res = vkCreateInstance(&instanceCreateInfo, VK_NULL_HANDLE, &instance)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating instance! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
	}

renderer_vk_create_instance_fail:
	MARS_FREE(extensions);
	return instance;
}

void _RendererVKDestroyInstance(VkInstance* _instance) {
	if (_instance) {
		vkDestroyInstance(*_instance, VK_NULL_HANDLE);
	}
}

VkDevice _RendererVKCreateDevice(VkPhysicalDevice* _physicalDevice, uint32_t _numQueueFamily, VkQueueFamilyProperties* _queueFamilyProperties) {
	MARS_RETURN_CLEAR;
	float** queuePriorities = NULL;
	VkDeviceQueueCreateInfo* deviceQueueCreateInfo = NULL;
	VkDevice device = VK_NULL_HANDLE;

	// Error check
	if (!_physicalDevice) {
		MARS_DEBUG_WARN("Invalid physical device reference!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		goto renderer_vk_create_device_fail;
	}
	if (!_queueFamilyProperties) {
		MARS_DEBUG_WARN("Invalid queue device properties reference!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		goto renderer_vk_create_device_fail;
	}

	// Populate creation info structs
	queuePriorities = MARS_MALLOC(_numQueueFamily * sizeof(*queuePriorities));
	if (!queuePriorities) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate queue priorities double buffer!");
		goto renderer_vk_create_device_fail;
	}
	deviceQueueCreateInfo = MARS_MALLOC(_numQueueFamily * sizeof(*deviceQueueCreateInfo));
	if (!deviceQueueCreateInfo) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate queue creation info buffer!");
		goto renderer_vk_create_device_fail;
	}
	for(uint32_t i=0; i<_numQueueFamily; ++i) {
		queuePriorities[i] = MARS_MALLOC(_queueFamilyProperties[i].queueCount * sizeof(*queuePriorities[i]));
		if (!queuePriorities[i]) {
			MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate queue priorities buffer (%d)!", (int)i);
			goto renderer_vk_create_device_fail;
		}
		for(uint32_t j=0; j<_queueFamilyProperties[i].queueCount; ++j) {
			queuePriorities[i][j] = 1.f;
		}

		deviceQueueCreateInfo[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfo[i].pNext = VK_NULL_HANDLE;
		deviceQueueCreateInfo[i].flags = 0;
		deviceQueueCreateInfo[i].queueFamilyIndex = i;
		deviceQueueCreateInfo[i].queueCount = _queueFamilyProperties[i].queueCount;
		deviceQueueCreateInfo[i].pQueuePriorities = queuePriorities[i];
	}
	const char extensionList[][VK_MAX_EXTENSION_NAME_SIZE] = {
		"VK_KHR_swapchain"
	};
	const char* extensions[] = {
		extensionList[0]
	};
	VkPhysicalDeviceFeatures physicalDeviceFeatures;
	vkGetPhysicalDeviceFeatures(*_physicalDevice, &physicalDeviceFeatures);
	VkDeviceCreateInfo deviceCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		_numQueueFamily,
		deviceQueueCreateInfo,
		0,
		VK_NULL_HANDLE,
		1,
		extensions,
		&physicalDeviceFeatures
	};

	// Create device
	VkResult res;
	if ((res = vkCreateDevice(*_physicalDevice, &deviceCreateInfo, VK_NULL_HANDLE, &device)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating device! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
		goto renderer_vk_create_device_fail;
	}

renderer_vk_create_device_fail:
	if (queuePriorities) {
		for(uint32_t i = 0; i < _numQueueFamily; ++i) {
			MARS_FREE(queuePriorities[i]);
		}
	}
	MARS_FREE(queuePriorities);
	MARS_FREE(deviceQueueCreateInfo);
	return device;
}

void _RendererVKDestroyDevice(VkDevice* _device) {
	if (_device) {
		vkDestroyDevice(*_device, VK_NULL_HANDLE);
	}
}

uint32_t _RendererVKGetPhysicalDeviceNumber(VkInstance* _instance) {
	MARS_RETURN_CLEAR;

	// Error check
	if (!_instance) {
		MARS_DEBUG_WARN("Invalid vulkan instance!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		return 0;
	}

	// Retrieve device info
	VkResult res;
	uint32_t physicalDeviceNumber = 0;
	if ((res = vkEnumeratePhysicalDevices(*_instance, &physicalDeviceNumber, VK_NULL_HANDLE)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error enumerating physical devices! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
		return 0;
	}
	return physicalDeviceNumber;
}

VkPhysicalDevice* _RendererVKGetPhysicalDevices(VkInstance* _instance, uint32_t _numDevices) {
	MARS_RETURN_CLEAR;
	VkPhysicalDevice* physicalDevices = NULL;

	// Error check
	if (!_instance) {
		MARS_DEBUG_WARN("Invalid vulkan instance!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		goto renderer_vk_get_physical_devices_fail;
	}
	if (_numDevices == 0) {
		MARS_DEBUG_WARN("Number of specified devices must be greater than zero!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_PARAMETER);
		goto renderer_vk_get_physical_devices_fail;
	}

	// Allocate buffers
	physicalDevices = MARS_MALLOC(_numDevices * sizeof(*physicalDevices));
	if (!physicalDevices) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate physical device array!");
		goto renderer_vk_get_physical_devices_fail;
	}

	// Retrieve device info
	VkResult res;
	if ((res = vkEnumeratePhysicalDevices(*_instance, &_numDevices, physicalDevices)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error retrieving physical devices! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
		goto renderer_vk_get_physical_devices_fail;
	}
	return physicalDevices;

renderer_vk_get_physical_devices_fail:
	MARS_FREE(physicalDevices);
	return NULL;
}

void _RendererVKDestroyPhysicalDevices(VkPhysicalDevice** _physicalDevices) {
	MARS_FREE(*_physicalDevices);
}

uint32_t _RendererVKGetBestPhysicalDeviceIndex(VkPhysicalDevice* _physicalDevices, uint32_t _numDevices) {
	MARS_RETURN_CLEAR;
	VkPhysicalDeviceProperties* physicalDeviceProperties = NULL;
	VkPhysicalDeviceMemoryProperties* physicalDeviceMemoryProperties = NULL;
	uint32_t* discreteGPUIndices = NULL;
	uint32_t* integratedGPUIndices = NULL;

	// Error check
	if (!_physicalDevices) {
		MARS_DEBUG_WARN("Invalid physical device buffer!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		goto renderer_vk_get_best_physical_device_index_fail;
	}
	if (_numDevices == 0) {
		MARS_DEBUG_WARN("Number of specified devices must be greater than zero!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_PARAMETER);
		goto renderer_vk_get_best_physical_device_index_fail;
	}

	// Allocate buffers
	physicalDeviceProperties = MARS_MALLOC(_numDevices * sizeof(*physicalDeviceProperties));
	if (!physicalDeviceProperties) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate physical device property buffer!");
		goto renderer_vk_get_best_physical_device_index_fail;
	}
	physicalDeviceMemoryProperties = MARS_MALLOC(_numDevices * sizeof(*physicalDeviceMemoryProperties));
	if (!physicalDeviceMemoryProperties) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate physical device memory property buffer!");
		goto renderer_vk_get_best_physical_device_index_fail;
	}
	discreteGPUIndices = MARS_MALLOC(_numDevices * sizeof(*discreteGPUIndices));
	if (!discreteGPUIndices) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate discrete GPU index buffer!");
		goto renderer_vk_get_best_physical_device_index_fail;
	}
	integratedGPUIndices = MARS_MALLOC(_numDevices * sizeof(*integratedGPUIndices));
	if (!integratedGPUIndices) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate integrated GPU index buffer!");
		goto renderer_vk_get_best_physical_device_index_fail;
	}

	// Get physical devices
	uint32_t numDiscreteGPU = 0;
	uint32_t numIntegratedGPU = 0;
	for(uint32_t i=0; i<_numDevices; ++i) {
		vkGetPhysicalDeviceProperties(_physicalDevices[i], &physicalDeviceProperties[i]);
		vkGetPhysicalDeviceMemoryProperties(_physicalDevices[i], &physicalDeviceMemoryProperties[i]);

		if (physicalDeviceProperties[i].deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			discreteGPUIndices[numDiscreteGPU++] = i;
		}
		else if (physicalDeviceProperties[i].deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
			integratedGPUIndices[numIntegratedGPU++] = i;
		}
	}

	// Score devices
	uint32_t bestPhysicalDeviceIdx = 0;
	VkDeviceSize bestPhysicalDeviceMemory = 0;
	for(uint32_t i=0; i<numDiscreteGPU; ++i) {
		uint32_t deviceIdx = discreteGPUIndices[i];
		if (bestPhysicalDeviceMemory < _RendererVKGetPhysicalDeviceTotalMemory(&physicalDeviceMemoryProperties[deviceIdx])) {
			bestPhysicalDeviceIdx = deviceIdx;
		}
	}
	for(uint32_t i=0; i<numIntegratedGPU; ++i) {
		uint32_t deviceIdx = integratedGPUIndices[i];
		if (bestPhysicalDeviceMemory < _RendererVKGetPhysicalDeviceTotalMemory(&physicalDeviceMemoryProperties[deviceIdx])) {
			bestPhysicalDeviceIdx = deviceIdx;
		}
	}
	return bestPhysicalDeviceIdx;

renderer_vk_get_best_physical_device_index_fail:
	MARS_FREE(physicalDeviceProperties);
	MARS_FREE(physicalDeviceMemoryProperties);
	MARS_FREE(discreteGPUIndices);
	MARS_FREE(integratedGPUIndices);
	return 0;
}

uint32_t _RendererVKGetPhysicalDeviceTotalMemory(VkPhysicalDeviceMemoryProperties* _properties) {
	MARS_RETURN_CLEAR;

	// Error check
	if (!_properties) {
		MARS_DEBUG_WARN("Invalid physical device proeprty buffer!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		return 0;
	}

	// Add up available memory
	uint32_t physicalDeviceTotalMemory = 0;
	for(uint32_t i=0; i<_properties->memoryHeapCount; ++i) {
		if ((_properties->memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0) {
			physicalDeviceTotalMemory += (uint32_t)_properties->memoryHeaps[i].size;
		}
	}
	return physicalDeviceTotalMemory;
}

uint32_t _RendererVKGetQueueFamilyNumber(VkPhysicalDevice* _physicalDevice) {
	MARS_RETURN_CLEAR;

	// Error check
	if (!_physicalDevice) {
		MARS_DEBUG_WARN("Invalid physical device reference!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		return 0;
	}

	// Retrieve queue info
	uint32_t queueFamilyNumber = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(*_physicalDevice, &queueFamilyNumber, VK_NULL_HANDLE);
	return queueFamilyNumber;
}

VkQueueFamilyProperties* _RendererVKGetQueueFamilyProperties(VkPhysicalDevice* _physicalDevice, uint32_t _numQueueFamily) {
	MARS_RETURN_CLEAR;
	VkQueueFamilyProperties* queueFamilyProperties = NULL;

	// Error check
	if (!_physicalDevice) {
		MARS_DEBUG_WARN("Invalid physical device reference!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		goto renderer_vk_get_queue_family_properties_fail;
	}
	if (_numQueueFamily == 0) {
		MARS_DEBUG_WARN("Number of specified devices must be greater than zero!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_PARAMETER);
		goto renderer_vk_get_queue_family_properties_fail;
	}

	// Retrieve queue info
	queueFamilyProperties = MARS_MALLOC(_numQueueFamily * sizeof(*queueFamilyProperties));
	if (!queueFamilyProperties) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate queue family property buffer!");
		goto renderer_vk_get_queue_family_properties_fail;
	}
	vkGetPhysicalDeviceQueueFamilyProperties(*_physicalDevice, &_numQueueFamily, queueFamilyProperties);
	return queueFamilyProperties;

renderer_vk_get_queue_family_properties_fail:
	MARS_FREE(queueFamilyProperties);
	return NULL;
}

void _RendererVKDestroyQueueFamilyProperties(VkQueueFamilyProperties** _queueFamilyProperties) {
	if (_queueFamilyProperties) {
		MARS_FREE(*_queueFamilyProperties);
	}
}

uint32_t _RendererVKGetBestGraphicsQueueFamilyIndex(VkQueueFamilyProperties* _queueFamilyProperties, uint32_t _numQueueFamily) {
	MARS_RETURN_CLEAR;
	uint32_t bestGraphicsQueueFamilyCount = 0;
	uint32_t bestGraphicsQueueFamilyIdx = 0;
	uint32_t* graphicsQueueFamilyIndices = NULL;

	// Error check
	if (!_queueFamilyProperties) {
		MARS_DEBUG_WARN("Invalid queue family properties reference!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		goto renderer_vk_get_best_graphics_queue_family_index_fail;
	}

	// Allocate index buffer
	graphicsQueueFamilyIndices = MARS_MALLOC(_numQueueFamily * sizeof(*graphicsQueueFamilyIndices));
	if (!graphicsQueueFamilyIndices) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate graphics queue family index buffer!");
		goto renderer_vk_get_best_graphics_queue_family_index_fail;
	}

	// Score queue families
	uint32_t numGraphicsFamilyQueues = 0;
	for(uint32_t i=0; i<_numQueueFamily; ++i) {
		if ((_queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
			graphicsQueueFamilyIndices[numGraphicsFamilyQueues++] = i;
		}
	}
	for(uint32_t i=0; i<numGraphicsFamilyQueues; ++i) {
		uint32_t queueCount = _queueFamilyProperties[graphicsQueueFamilyIndices[i]].queueCount;
		if (queueCount > bestGraphicsQueueFamilyCount) {
			bestGraphicsQueueFamilyCount = queueCount;
			bestGraphicsQueueFamilyIdx = i;
		}
	}

renderer_vk_get_best_graphics_queue_family_index_fail:
	MARS_FREE(graphicsQueueFamilyIndices);
	return bestGraphicsQueueFamilyIdx;
}

uint32_t _RendererVKGetGraphicsQueueMode(VkQueueFamilyProperties* _queueFamilyProperties, uint32_t _graphicsQueueFamiltyIdx) {
	MARS_RETURN_CLEAR;

	// Error check
	if (!_queueFamilyProperties) {
		MARS_DEBUG_WARN("Invalid queue family properties reference!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		return 2;
	}

	// Check properties
	uint32_t queueCount = _queueFamilyProperties[_graphicsQueueFamiltyIdx].queueCount;
	if (queueCount == 1) { return 0; }
	else if (queueCount > 1) { return 1; }
	else { return 2; }
}

VkQueue _RendererVKGetDrawingQueue(VkDevice* _device, uint32_t _graphicsQueueFamilyIdx) {
	MARS_RETURN_CLEAR;
	VkQueue drawingQueue = VK_NULL_HANDLE;

	// Error check
	if (!_device) {
		MARS_DEBUG_WARN("Invalid device reference!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		return drawingQueue;
	}

	// Retrieve queue
	vkGetDeviceQueue(*_device, _graphicsQueueFamilyIdx, 0, &drawingQueue);
	return drawingQueue;
}

VkQueue _RendererVKGetPresentingQueue(VkDevice* _device, uint32_t _graphicsQueueFamilyIdx, uint32_t _graphicsQueueMode) {
	MARS_RETURN_CLEAR;
	VkQueue presentingQueue = VK_NULL_HANDLE;

	// Error check
	if (!_device) {
		MARS_DEBUG_WARN("Invalid device reference!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		return presentingQueue;
	}
	if (_graphicsQueueMode > 1) {
		MARS_DEBUG_WARN("Invalid graphics queue mode!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_PARAMETER);
		return presentingQueue;
	}

	// Retrieve queue
	vkGetDeviceQueue(*_device, _graphicsQueueFamilyIdx, _graphicsQueueMode, &presentingQueue);
	return presentingQueue;
}

VkSurfaceKHR _RendererVKCreateSurface(GLFWwindow* _window, VkInstance* _instance) {
	MARS_RETURN_CLEAR;
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkResult res;
	if ((res = glfwCreateWindowSurface(*_instance, _window, VK_NULL_HANDLE, &surface)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating window surface! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
	}
	return surface;
}

void _RendererVKDestroySurface(VkSurfaceKHR* _surface, VkInstance* _instance) {
	vkDestroySurfaceKHR(*_instance, *_surface, VK_NULL_HANDLE);
}

VkBool32 _RendererVKGetSurfaceSupport(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice, uint32_t _graphicsQueueFamilyIdx) {
	MARS_RETURN_CLEAR;
	VkBool32 surfaceSupport = 0;
	VkResult res;
	if ((res = vkGetPhysicalDeviceSurfaceSupportKHR(*_physicalDevice, _graphicsQueueFamilyIdx, *_surface, &surfaceSupport)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error getting surface support! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
	}
	return surfaceSupport;
}

VkSurfaceCapabilitiesKHR _RendererVKGetSurfaceCapabilities(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice) {
	MARS_RETURN_CLEAR;
	VkSurfaceCapabilitiesKHR surfaceCapabilities = { 0 };
	VkResult res;
	if ((res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*_physicalDevice, *_surface, &surfaceCapabilities)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error getting surface capabilities! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
	}
	return surfaceCapabilities;
}

VkSurfaceFormatKHR _RendererVKGetBestSurfaceFormat(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice) {
	MARS_RETURN_CODE;
	uint32_t numSurfaceFormat = 0;
	VkSurfaceFormatKHR* surfaceFormats = NULL;
	VkSurfaceFormatKHR bestSurfaceFormat = { 0 };

	// Get number of surface formats
	VkResult res;
	if ((res = vkGetPhysicalDeviceSurfaceFormatsKHR(*_physicalDevice, *_surface, &numSurfaceFormat, VK_NULL_HANDLE)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error enumerating surface formats! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
		goto renderer_vk_get_best_surface_format_fail;
	}

	// Get all surface formats
	surfaceFormats = MARS_MALLOC(numSurfaceFormat * sizeof(*surfaceFormats));
	if (!surfaceFormats) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate surface formats buffer!");
		goto renderer_vk_get_best_surface_format_fail;
	}
	if ((res = vkGetPhysicalDeviceSurfaceFormatsKHR(*_physicalDevice, *_surface, &numSurfaceFormat, surfaceFormats)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error retrieving surface formats! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
		goto renderer_vk_get_best_surface_format_fail;
	}
	bestSurfaceFormat = surfaceFormats[0];

renderer_vk_get_best_surface_format_fail:
	MARS_FREE(surfaceFormats);
	return bestSurfaceFormat;

}

VkPresentModeKHR _RendererVKGetBestPresentMode(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice) {
	MARS_RETURN_CLEAR;
	uint32_t numPresentMode = 0;
	VkPresentModeKHR* presentModes = NULL;
	VkPresentModeKHR bestPresentMode = VK_PRESENT_MODE_FIFO_KHR;
	VkResult res;

	// Get number of present modes
	if ((res = vkGetPhysicalDeviceSurfacePresentModesKHR(*_physicalDevice, *_surface, &numPresentMode, VK_NULL_HANDLE)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error enumerating present modes! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
		goto renderer_vk_get_best_present_mode_fail;
	}

	// Get present modes
	presentModes = MARS_MALLOC(numPresentMode * sizeof(*presentModes));
	if (!presentModes) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate present modes buffer!");
		goto renderer_vk_get_best_present_mode_fail;
	}
	if ((res = vkGetPhysicalDeviceSurfacePresentModesKHR(*_physicalDevice, *_surface, &numPresentMode, presentModes)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error retrieving present modes! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
		goto renderer_vk_get_best_present_mode_fail;
	}

	// Select best present mode
	for(uint32_t i=0; i<numPresentMode; ++i) {
		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
			bestPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		}
	}

renderer_vk_get_best_present_mode_fail:
	MARS_FREE(presentModes);
	return bestPresentMode;
}

VkExtent2D _RendererVKGetBestSwapchainExtent(VkSurfaceCapabilitiesKHR* _surfaceCapabilities, GLFWwindow* _window) {
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	glfwGetFramebufferSize(_window, &framebufferWidth, &framebufferHeight);
	VkExtent2D bestSwapchainExtent;

	// Get max width
	if (_surfaceCapabilities->currentExtent.width < (uint32_t)framebufferWidth) {
		bestSwapchainExtent.width = _surfaceCapabilities->currentExtent.width;
	}
	else {
		bestSwapchainExtent.width = framebufferWidth;
	}

	// Get max height
	if (_surfaceCapabilities->currentExtent.height < (uint32_t)framebufferHeight) {
		bestSwapchainExtent.height = _surfaceCapabilities->currentExtent.height;
	}
	else {
		bestSwapchainExtent.height = framebufferHeight;
	}

	return bestSwapchainExtent;
}

VkSwapchainKHR _RendererVKCreateSwapchain(VkDevice* _device, VkSurfaceKHR* _surface, VkSurfaceCapabilitiesKHR* _surfaceCapabilities, VkSurfaceFormatKHR* _surfaceFormat, VkExtent2D* _swapChainExtent, VkPresentModeKHR* _presentMode, uint32_t _imageArrayLayers, uint32_t _graphicsQueueMode) {
	MARS_RETURN_CLEAR;
	VkSharingMode imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	uint32_t numQueueFamilyIndices = 0;
	uint32_t* queueFamilyIndicesPtr = VK_NULL_HANDLE;
	uint32_t queueFamilyIndices[] = {0, 1};

	// Determine image sharing mode
	if (_graphicsQueueMode == 1) {
		imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		numQueueFamilyIndices = 2;
		queueFamilyIndicesPtr = &queueFamilyIndices[0];
	}

	// Populate descriptor struct
	VkSwapchainCreateInfoKHR swapchainCreateInfo = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		VK_NULL_HANDLE,
		0,
		*_surface,
		_surfaceCapabilities->minImageCount + 1,
		_surfaceFormat->format,
		_surfaceFormat->colorSpace,
		*_swapChainExtent,
		_imageArrayLayers,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		imageSharingMode,
		numQueueFamilyIndices,
		queueFamilyIndicesPtr,
		_surfaceCapabilities->currentTransform,
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		*_presentMode,
		VK_TRUE,
		VK_NULL_HANDLE
	};

	// Create swapchain
	VkSwapchainKHR swapchain;
	VkResult res;
	if ((res = vkCreateSwapchainKHR(*_device, &swapchainCreateInfo, VK_NULL_HANDLE, &swapchain)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating swapchain! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
	}
	return swapchain;
}

void _RendererVKDestroySwapchain(VkDevice* _device, VkSwapchainKHR* _swapchain) {
	vkDestroySwapchainKHR(*_device, *_swapchain, VK_NULL_HANDLE);
}

uint32_t _RendererVKGetSwapchainImageNumber(VkDevice* _device, VkSwapchainKHR* _swapchain) {
	MARS_RETURN_CLEAR;
	uint32_t numSwapchainImages;
	VkResult res;
	if ((res = vkGetSwapchainImagesKHR(*_device, *_swapchain, &numSwapchainImages, VK_NULL_HANDLE)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error enumerating swapchain images! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
		return 0;
	}
	return numSwapchainImages;
}

VkImage* _RendererVKGetSwapchainImages(VkDevice* _device, VkSwapchainKHR* _swapchain, uint32_t _numSwapchainImages) {
	MARS_RETURN_CLEAR;
	VkImage* swapchainImages = MARS_MALLOC(_numSwapchainImages * sizeof(*swapchainImages));
	if (!swapchainImages) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate swapchain images buffer!");
		return NULL;
	}
	VkResult res;
	if ((res = vkGetSwapchainImagesKHR(*_device, *_swapchain, &_numSwapchainImages, swapchainImages)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error populating swapchain images! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
		return NULL;
	}
	return swapchainImages;
}

void _RendererVKDestroySwapchainImages(VkImage** _images) {
	MARS_FREE(*_images);
}

VkImageView* _RendererVKCreateImageViews(VkDevice* _device, VkImage** _images, VkSurfaceFormatKHR* _format, uint32_t _numImages, uint32_t _imageArrayLayers) {
	MARS_RETURN_CLEAR;
	VkImageViewCreateInfo* imageViewCreateInfo = NULL;
	VkImageView* imageViews = NULL;
	VkComponentMapping componentMapping = {
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY,
		VK_COMPONENT_SWIZZLE_IDENTITY
	};

	VkImageSubresourceRange imageSubresourceRange = {
		VK_IMAGE_ASPECT_COLOR_BIT,
		0,
		1,
		0,
		_imageArrayLayers
	};

	imageViewCreateInfo = MARS_MALLOC(_numImages * sizeof(*imageViewCreateInfo));
	if (!imageViewCreateInfo) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate image view create info buffer!");
		goto renderer_vk_create_image_views_fail;
	}
	imageViews = MARS_MALLOC(_numImages * sizeof(*imageViews));
	if (!imageViews) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate image view buffer!");
		goto renderer_vk_create_image_views_fail;
	}
	for(uint32_t i = 0; i < _numImages; ++i) {
		imageViewCreateInfo[i].sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo[i].pNext = VK_NULL_HANDLE;
		imageViewCreateInfo[i].flags = 0;
		imageViewCreateInfo[i].image = (*_images)[i];
		imageViewCreateInfo[i].viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo[i].format = _format->format;
		imageViewCreateInfo[i].components = componentMapping;
		imageViewCreateInfo[i].subresourceRange = imageSubresourceRange;

		VkResult res;
		if ((res = vkCreateImageView(*_device, &(imageViewCreateInfo[i]), VK_NULL_HANDLE, &(imageViews[i]))) != VK_SUCCESS) {
			MARS_DEBUG_WARN("Vulkan error creating swapchain images! (%d)", (int)res);
			MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
			goto renderer_vk_create_image_views_fail;
		}
	}

	MARS_FREE(imageViewCreateInfo);
	return imageViews;

renderer_vk_create_image_views_fail:
	MARS_FREE(imageViewCreateInfo);
	MARS_FREE(imageViews);
	return NULL;
}

void _RendererVKDestroyImageViews(VkDevice* _device, VkImageView** _imageViews, uint32_t _numImageViews) {
	for(uint32_t i = 0; i < _numImageViews; ++i) {
		vkDestroyImageView(*_device, (*_imageViews)[i], VK_NULL_HANDLE);
	}
}

VkRenderPass _RendererVKCreateRenderPass(VkDevice* _device, VkSurfaceFormatKHR* _format) {
	MARS_RETURN_CLEAR;
	VkAttachmentDescription attachmentDescription = {
		0,
		_format->format,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	VkAttachmentReference attachmentReference = {
		0,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	VkSubpassDescription subpassDescription = {
		0,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		0,
		VK_NULL_HANDLE,
		1,
		&attachmentReference,
		VK_NULL_HANDLE,
		VK_NULL_HANDLE,
		0,
		VK_NULL_HANDLE
	};

	VkSubpassDependency subpassDependency = {
		VK_SUBPASS_EXTERNAL,
		0,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		0,
		VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		0
	};

	VkRenderPassCreateInfo renderPassCreateInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		1,
		&attachmentDescription,
		1,
		&subpassDescription,
		1,
		&subpassDependency
	};

	VkRenderPass renderPass;
	VkResult res;
	if ((res = vkCreateRenderPass(*_device, &renderPassCreateInfo, VK_NULL_HANDLE, &renderPass)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating render pass! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
	}
	return renderPass;
}

void _RendererVKDestroyRenderPass(VkDevice* _device, VkRenderPass *_renderPass) {
	vkDestroyRenderPass(*_device, *_renderPass, VK_NULL_HANDLE);
}

VkFramebuffer* _RendererVKCreateFramebuffers(VkDevice *_device, VkRenderPass* _renderPass, VkExtent2D *_extent, VkImageView** _imageViews, uint32_t _numImageViews) {
	MARS_RETURN_CLEAR;
	VkFramebufferCreateInfo* framebufferCreateInfo = NULL;
	VkFramebuffer* framebuffers = NULL;

	// Allocate framebuffer arrays
	framebufferCreateInfo = MARS_MALLOC(_numImageViews * sizeof(*framebufferCreateInfo));
	if (!framebufferCreateInfo) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate framebuffer create info buffer!");
		goto renderer_vk_create_framebuffers_fail;
	}
	framebuffers = MARS_MALLOC(_numImageViews * sizeof((framebuffers)));
	if (!framebuffers) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate framebuffer buffer!");
		goto renderer_vk_create_framebuffers_fail;
	}

	// Populate create info
	for(uint32_t i = 0; i < _numImageViews; ++i) {
		framebufferCreateInfo[i].sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo[i].pNext = VK_NULL_HANDLE;
		framebufferCreateInfo[i].flags = 0;
		framebufferCreateInfo[i].renderPass = *_renderPass;
		framebufferCreateInfo[i].attachmentCount = 1;
		framebufferCreateInfo[i].pAttachments = &(*_imageViews)[i];
		framebufferCreateInfo[i].width = _extent->width;
		framebufferCreateInfo[i].height = _extent->height;
		framebufferCreateInfo[i].layers = 1;

		VkResult res;
		if ((res = vkCreateFramebuffer(*_device, &framebufferCreateInfo[i], VK_NULL_HANDLE, &framebuffers[i])) != VK_SUCCESS) {
			MARS_DEBUG_WARN("Vulkan error creating framebuffer! (%d)", (int)res);
			MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
			goto renderer_vk_create_framebuffers_fail;
		}
	}

	MARS_FREE(framebufferCreateInfo);
	return framebuffers;

renderer_vk_create_framebuffers_fail:
	MARS_FREE(framebuffers);
	MARS_FREE(framebufferCreateInfo);
	return NULL;
}

void _RendererVKDestroyFramebuffers(VkDevice* _device, VkFramebuffer** _framebuffers, uint32_t _numFrameBuffers) {
	for(uint32_t i = 0; i < _numFrameBuffers; ++i) {
		vkDestroyFramebuffer(*_device, (*_framebuffers)[i], VK_NULL_HANDLE);
	}
	MARS_FREE(*_framebuffers);
}

char* _RendererVKGetShaderCode(const char* _filename, uint32_t* _shaderSize) {
	MARS_RETURN_CLEAR;
	char* shaderCode = NULL;
	FILE* fp = NULL;

	// Validate inputs
	if (!_shaderSize) {
		MARS_DEBUG_WARN("NULL file size destination pointer!");
		MARS_RETURN_SET(MARS_RETURN_CODE_INVALID_REFERENCE);
		goto renderer_vk_get_shader_code_fail;
	}

	// Open shader file
	fp = fopen(_filename, "rb+");
	if (!fp) {
		MARS_DEBUG_WARN("Failed to open shader file! (%s)", _filename);
		MARS_RETURN_SET(MARS_RETURN_CODE_FILESYSTEM_FAILURE);
		goto renderer_vk_get_shader_code_fail;
	}

	// Get file size
	fseek(fp, 0l, SEEK_END);
	*_shaderSize = (uint32_t)ftell(fp);
	rewind(fp);

	// Read shader code
	shaderCode = MARS_MALLOC((*_shaderSize) * sizeof(*shaderCode));
	if (!shaderCode) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate shader code buffer!");
		goto renderer_vk_get_shader_code_fail;
	}
	fread(shaderCode, sizeof(*shaderCode), *_shaderSize, fp);

	fclose(fp);
	return shaderCode;
renderer_vk_get_shader_code_fail:
	if (fp) fclose(fp);
	MARS_FREE(shaderCode);
	return NULL;
}

void _RendererVKDestroyShaderCode(char** _shaderCode) {
	MARS_FREE(*_shaderCode);
}

VkShaderModule _RendererVKCreateShaderModule(VkDevice* _device, char* _shaderCode, uint32_t _shaderSize) {
	MARS_RETURN_CLEAR;
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		_shaderSize,
		(const uint32_t*)_shaderCode
	};

	VkShaderModule shaderModule;
	VkResult res;
	if ((res = vkCreateShaderModule(*_device, &shaderModuleCreateInfo, VK_NULL_HANDLE, &shaderModule)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating shader module! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
	}
	return shaderModule;
}

void _RendererVKDestroyShaderModule(VkDevice* _device, VkShaderModule* _shaderModule) {
	vkDestroyShaderModule(*_device, *_shaderModule, VK_NULL_HANDLE);
}

VkPipelineLayout _RendererVKCreatePipelineLayout(VkDevice* _device) {
	MARS_RETURN_CLEAR;
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		0,
		VK_NULL_HANDLE,
		0,
		VK_NULL_HANDLE
	};

	VkPipelineLayout pipelineLayout;
	VkResult res;
	if ((res = vkCreatePipelineLayout(*_device, &pipelineLayoutCreateInfo, VK_NULL_HANDLE, &pipelineLayout)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating pipeline layout! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
	}
	return pipelineLayout;
}

void _RendererVKDestroyPipelineLayout(VkDevice* _device, VkPipelineLayout* _pipelineLayout) {
	vkDestroyPipelineLayout(*_device, *_pipelineLayout, VK_NULL_HANDLE);
}

VkPipelineShaderStageCreateInfo _RendererVKConfigureVertexShaderStageCreateInfo(VkShaderModule* _vertexShaderModule, const char* _entryName) {
	VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		VK_SHADER_STAGE_VERTEX_BIT,
		*_vertexShaderModule,
		_entryName,
		VK_NULL_HANDLE
	};

	return vertexShaderStageCreateInfo;
}

VkPipelineShaderStageCreateInfo _RendererVKConfigureFragmentShaderStageCreateInfo(VkShaderModule* _fragmentShaderModule, const char* _entryName) {
	VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		VK_SHADER_STAGE_FRAGMENT_BIT,
		*_fragmentShaderModule,
		_entryName,
		VK_NULL_HANDLE
	};

	return fragmentShaderStageCreateInfo;
}

VkPipelineVertexInputStateCreateInfo _RendererVKConfigureVertexInputStateCreateInfo() {
	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		0,
		VK_NULL_HANDLE,
		0,
		VK_NULL_HANDLE
	};

	return vertexInputStateCreateInfo;
}

VkPipelineInputAssemblyStateCreateInfo _RendererVKConfigureInputAssemblyStateCreateInfo() {
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		VK_FALSE
	};

	return inputAssemblyStateCreateInfo;
}

VkPipeline _RendererVKCreateGraphicsPipeline(VkDevice* _device, VkPipelineLayout* _pipelineLayout, VkShaderModule *_vertexShaderModule, VkShaderModule* _fragmentShaderModule, VkRenderPass* _renderPass, VkExtent2D* _extent) {
	MARS_RETURN_CLEAR;

	char entryName[] = "main";
	VkPipelineShaderStageCreateInfo shaderStageCreateInfo[] = {
		_RendererVKConfigureVertexShaderStageCreateInfo(_vertexShaderModule, entryName),
		_RendererVKConfigureFragmentShaderStageCreateInfo(_fragmentShaderModule, entryName)
	};
	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = _RendererVKConfigureVertexInputStateCreateInfo();
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = _RendererVKConfigureInputAssemblyStateCreateInfo();
	VkViewport viewport = _RendererVKConfigureViewport(_extent);
	VkRect2D scissor = _RendererVKConfigureScissor(_extent, 0, 0, 0, 0);
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = _RendererVKConfigureViewportStateCreateInfo(&viewport, &scissor);
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = _RendererVKConfigureRasterizationStateCreateInfo();
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = _RendererVKConfigureMultisampleStateCreateInfo();
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = _RendererVKConfigureColorBlendAttachmentState();
	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = _RendererVKConfigureColorBlendStateCreateInfo(&colorBlendAttachmentState);

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		2,
		shaderStageCreateInfo,
		&vertexInputStateCreateInfo,
		&inputAssemblyStateCreateInfo,
		VK_NULL_HANDLE,
		&viewportStateCreateInfo,
		&rasterizationStateCreateInfo,
		&multisampleStateCreateInfo,
		VK_NULL_HANDLE,
		&colorBlendStateCreateInfo,
		VK_NULL_HANDLE,
		*_pipelineLayout,
		*_renderPass,
		0,
		VK_NULL_HANDLE,
		-1
	};

	VkPipeline graphicsPipeline;
	VkResult res;
	if ((res = vkCreateGraphicsPipelines(*_device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, VK_NULL_HANDLE, &graphicsPipeline)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating graphics pipeline! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
	}
	return graphicsPipeline;
}

void _RendererVKDestroyGraphicsPipeline(VkDevice* _device, VkPipeline* _graphicsPipeline) {
	vkDestroyPipeline(*_device, *_graphicsPipeline, VK_NULL_HANDLE);
}

VkViewport _RendererVKConfigureViewport(VkExtent2D *_extent) {
	VkViewport viewport = {
		1.0f,
		1.0f,
		(float)_extent->width,
		(float)_extent->height,
		0.0f,
		1.0f
	};

	return viewport;
}

VkRect2D _RendererVKConfigureScissor(VkExtent2D *_extent, uint32_t _left, uint32_t _right, uint32_t _top, uint32_t _bottom) {
	if(_left > _extent->width){
		_left = _extent->width;
	}
	if(_right > _extent->width){
		_right = _extent->width;
	}
	if(_top > _extent->height){
		_top = _extent->height;
	}
	if(_bottom > _extent->height){
		_bottom = _extent->height;
	}
	VkOffset2D offset = {
		_left,
		_top
	};
	VkExtent2D extent = {
		_extent->width - _left - _right,
		_extent->height - _top - _bottom
	};
	VkRect2D scissor = {
		offset,
		extent
	};
	return scissor;
}

VkPipelineViewportStateCreateInfo _RendererVKConfigureViewportStateCreateInfo(VkViewport* _viewport, VkRect2D *_scissor) {
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		1,
		_viewport,
		1,
		_scissor
	};

	return viewportStateCreateInfo;
}

VkPipelineRasterizationStateCreateInfo _RendererVKConfigureRasterizationStateCreateInfo() {
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		VK_FALSE,
		VK_FALSE,
		VK_POLYGON_MODE_FILL,
		VK_CULL_MODE_BACK_BIT,
		VK_FRONT_FACE_CLOCKWISE,
		VK_FALSE,
		0.0f,
		0.0f,
		0.0f,
		1.0f
	};

	return rasterizationStateCreateInfo;
}

VkPipelineMultisampleStateCreateInfo _RendererVKConfigureMultisampleStateCreateInfo() {
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		VK_SAMPLE_COUNT_1_BIT,
		VK_FALSE,
		1.0f,
		VK_NULL_HANDLE,
		VK_FALSE,
		VK_FALSE
	};

	return multisampleStateCreateInfo;
}

VkPipelineColorBlendAttachmentState _RendererVKConfigureColorBlendAttachmentState() {
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
		VK_FALSE,
		VK_BLEND_FACTOR_ONE,
		VK_BLEND_FACTOR_ZERO,
		VK_BLEND_OP_ADD,
		VK_BLEND_FACTOR_ONE,
		VK_BLEND_FACTOR_ZERO,
		VK_BLEND_OP_ADD,
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
	};

	return colorBlendAttachmentState;
}

VkPipelineColorBlendStateCreateInfo _RendererVKConfigureColorBlendStateCreateInfo(VkPipelineColorBlendAttachmentState* _colorBlendAttachmentState) {
	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		VK_FALSE,
		VK_LOGIC_OP_COPY,
		1,
		_colorBlendAttachmentState,
		{0.0f, 0.0f, 0.0f, 0.0f}
	};

	return colorBlendStateCreateInfo;
}

VkCommandPool _RendererVKCreateCommandPool(VkDevice* _device, uint32_t _queueFamilyIndex) {
	MARS_RETURN_CLEAR;
	VkCommandPoolCreateInfo commandPoolCreateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		VK_NULL_HANDLE,
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		_queueFamilyIndex
	};

	VkCommandPool commandPool;
	VkResult res;
	if ((res = vkCreateCommandPool(*_device, &commandPoolCreateInfo, VK_NULL_HANDLE, &commandPool)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating command pool! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
	}
	return commandPool;
}

void _RendererVKDestroyCommandPool(VkDevice* _device, VkCommandPool* _commandPool) {
	vkDestroyCommandPool(*_device, *_commandPool, VK_NULL_HANDLE);
}

VkCommandBuffer* _RendererVKCreateCommandBuffers(VkDevice* _device, VkCommandPool* _commandPool, uint32_t _numCommandBuffers) {
	MARS_RETURN_CLEAR;
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		VK_NULL_HANDLE,
		*_commandPool,
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		_numCommandBuffers
	};

	VkCommandBuffer* commandBuffers = MARS_MALLOC(_numCommandBuffers * sizeof(*commandBuffers));
	if (!commandBuffers) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate command buffers!");
	}
	VkResult res;
	if ((res = vkAllocateCommandBuffers(*_device, &commandBufferAllocateInfo, commandBuffers)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating command buffers! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
	}
	return commandBuffers;
}

void _RendererVKDestroyCommandBuffers(VkDevice* _device, VkCommandBuffer** _commandBuffers, VkCommandPool* _commandPool, uint32_t _numCommandBuffers) {
	vkFreeCommandBuffers(*_device, *_commandPool, _numCommandBuffers, *_commandBuffers);
	MARS_FREE(*_commandBuffers);
}

void _RendererVKRecordCommandBuffers(VkCommandBuffer** _commandBuffers, VkRenderPass* _renderPass, VkFramebuffer** _framebuffers, VkExtent2D* _extent, VkPipeline* _pipeline, uint32_t _numCommandBuffers) {
	MARS_RETURN_CLEAR;
	VkCommandBufferBeginInfo* commandBufferBeginInfos = NULL;
	VkRenderPassBeginInfo* renderPassBeginInfos = NULL;
	VkRect2D renderArea = {
		{0, 0},
		{_extent->width, _extent->height}
	};
	VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 0.0f};

	commandBufferBeginInfos = MARS_MALLOC(_numCommandBuffers * sizeof(*commandBufferBeginInfos));
	if (!commandBufferBeginInfos) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate command buffer info buffer!");
		goto renderer_vk_record_command_buffers_fail;
	}
	renderPassBeginInfos = MARS_MALLOC(_numCommandBuffers * sizeof(*renderPassBeginInfos));
	if (!renderPassBeginInfos) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate render pass info buffer!");
		goto renderer_vk_record_command_buffers_fail;
	}

	for(uint32_t i = 0; i < _numCommandBuffers; ++i) {
		commandBufferBeginInfos[i].sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfos[i].pNext = VK_NULL_HANDLE;
		commandBufferBeginInfos[i].flags = 0;
		commandBufferBeginInfos[i].pInheritanceInfo = VK_NULL_HANDLE;

		renderPassBeginInfos[i].sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfos[i].pNext = VK_NULL_HANDLE;
		renderPassBeginInfos[i].renderPass = *_renderPass;
		renderPassBeginInfos[i].framebuffer = (*_framebuffers)[i];
		renderPassBeginInfos[i].renderArea = renderArea;
		renderPassBeginInfos[i].clearValueCount = 1;
		renderPassBeginInfos[i].pClearValues = &clearValue;

		VkResult res;
		if ((res = vkBeginCommandBuffer((*_commandBuffers)[i], &commandBufferBeginInfos[i])) != VK_SUCCESS) {
			MARS_DEBUG_WARN("Vulkan error starting command submission! (%d)", (int)res);
			MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
			goto renderer_vk_record_command_buffers_fail;
		}
		vkCmdBeginRenderPass((*_commandBuffers)[i], &renderPassBeginInfos[i], VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline((*_commandBuffers)[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *_pipeline);
		vkCmdDraw((*_commandBuffers)[i], 3, 1, 0, 0);
		vkCmdEndRenderPass((*_commandBuffers)[i]);
		if ((res = vkEndCommandBuffer((*_commandBuffers)[i])) != VK_SUCCESS) {
			MARS_DEBUG_WARN("Vulkan error ending command submission! (%d)", (int)res);
			MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
			goto renderer_vk_record_command_buffers_fail;
		}
	}

renderer_vk_record_command_buffers_fail:
	MARS_FREE(commandBufferBeginInfos);
	MARS_FREE(renderPassBeginInfos);
}

VkSemaphore* _RendererVKCreateSemaphores(VkDevice* _device, uint32_t _maxFrames) {
	MARS_RETURN_CLEAR;
	VkSemaphore* semaphores = NULL;

	VkSemaphoreCreateInfo semaphoreCreateInfo = {
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		VK_NULL_HANDLE,
		0
	};
	semaphores = MARS_MALLOC(_maxFrames * sizeof(*semaphores));
	if (!semaphores) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate semaphores buffer!");
		goto renderer_vk_create_semaphores_fail;
	}
	for(uint32_t i = 0; i < _maxFrames; ++i) {
		VkResult res;
		if ((res = vkCreateSemaphore(*_device, &semaphoreCreateInfo, VK_NULL_HANDLE, &semaphores[i])) != VK_SUCCESS) {
			MARS_DEBUG_WARN("Vulkan error creating semaphore! (%d)", (int)res);
			MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
			goto renderer_vk_create_semaphores_fail;
		}
	}
	return semaphores;

renderer_vk_create_semaphores_fail:
	MARS_FREE(semaphores);
	return NULL;
}

void _RendererVKDestroySemaphores(VkDevice* _device, VkSemaphore** _semaphores, uint32_t _maxFrames) {
	for(uint32_t i = 0; i < _maxFrames; ++i) {
		vkDestroySemaphore(*_device, (*_semaphores)[i], VK_NULL_HANDLE);
	}
	MARS_FREE(*_semaphores);
}

VkFence* _RendererVKCreateFences(VkDevice* _device, uint32_t _maxFrames) {
	MARS_RETURN_CLEAR;
	VkFence* fences = NULL;

	VkFenceCreateInfo fenceCreateInfo = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		VK_NULL_HANDLE,
		VK_FENCE_CREATE_SIGNALED_BIT
	};
	fences = MARS_MALLOC(_maxFrames * sizeof(*fences));
	if (!fences) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate fences buffer!");
		goto renderer_vk_create_fences_fail;
	}
	for(uint32_t i = 0; i < _maxFrames; ++i) {
		VkResult res;
		if ((res = vkCreateFence(*_device, &fenceCreateInfo, VK_NULL_HANDLE, &fences[i])) != VK_SUCCESS) {
			MARS_DEBUG_WARN("Vulkan error creating fence! (%d)", (int)res);
			MARS_RETURN_SET(MARS_RETURN_CODE_BACKEND_FAILURE);
			goto renderer_vk_create_fences_fail;
		}
	}
	return fences;

renderer_vk_create_fences_fail:
	MARS_FREE(fences);
	return NULL;
}

void _RendererVKDestroyFences(VkDevice* _device, VkFence** _fences, uint32_t _maxFrames) {
	for(uint32_t i = 0; i < _maxFrames; ++i) {
		vkDestroyFence(*_device, (*_fences)[i], VK_NULL_HANDLE);
	}
	MARS_FREE(*_fences);
}

VkFence* _RendererVKCreateEmptyFences(uint32_t _maxFrames) {
	VkFence* fences = MARS_MALLOC(_maxFrames * sizeof(*fences));
	if (!fences) {
		MARS_ABORT(MARS_ERROR_CODE_BAD_ALLOC, "Failed to allocate fences buffer!");
		return NULL;
	}
	for(uint32_t i = 0; i < _maxFrames; ++i) {
		fences[i] = VK_NULL_HANDLE;
	}
	return fences;
}

void _RendererVKDestroyEmptyFences(VkFence** _fences) {
	MARS_FREE(*_fences);
}