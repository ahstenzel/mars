#include "mars/renderer.h"

VkInstance _RenderCreateInstance() {
	MARS_RETURN_CLEAR;

	// Define application parameters
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
	uint32_t numExtensions = 0;
	const char** extensions = glfwGetRequiredInstanceExtensions(&numExtensions);
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

	// Create instance
	VkInstance instance;
	VkResult res;
	if ((res = vkCreateInstance(&instanceCreateInfo, VK_NULL_HANDLE, &instance)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating instance! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
	}
	return instance;
}

void _RenderDestroyInstance(VkInstance* _instance) {
	if (_instance) {
		vkDestroyInstance(*_instance, VK_NULL_HANDLE);
	}
}

VkDevice _RenderCreateDevice(VkPhysicalDevice* _physicalDevice, uint32_t _numQueueFamily, VkQueueFamilyProperties* _queueFamilyProperties) {
	MARS_RETURN_CLEAR;
	float** queuePriorities = NULL;
	VkDeviceQueueCreateInfo* deviceQueueCreateInfo = NULL;
	VkDevice device = VK_NULL_HANDLE;

	// Error check
	if (!_physicalDevice) {
		MARS_DEBUG_WARN("Invalid physical device reference!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		goto render_create_device_fail;
	}
	if (!_queueFamilyProperties) {
		MARS_DEBUG_WARN("Invalid queue device properties reference!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		goto render_create_device_fail;
	}

	// Populate creation info structs
	queuePriorities = MARS_MALLOC(_numQueueFamily * sizeof(*queuePriorities));
	if (!queuePriorities) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate queue priorities double buffer!");
		goto render_create_device_fail;
	}
	deviceQueueCreateInfo = MARS_MALLOC(_numQueueFamily * sizeof(*deviceQueueCreateInfo));
	if (!deviceQueueCreateInfo) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate queue creation info buffer!");
		goto render_create_device_fail;
	}
	for(uint32_t i=0; i<_numQueueFamily; ++i) {
		queuePriorities[i] = MARS_MALLOC(_queueFamilyProperties[i].queueCount * sizeof(*queuePriorities[i]));
		if (!queuePriorities[i]) {
			MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate queue priorities buffer (%d)!", (int)i);
			goto render_create_device_fail;
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
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
		goto render_create_device_fail;
	}

render_create_device_fail:
	if (queuePriorities) {
		for(uint32_t i = 0; i < _numQueueFamily; ++i) {
			MARS_FREE(queuePriorities[i]);
		}
	}
	MARS_FREE(queuePriorities);
	MARS_FREE(deviceQueueCreateInfo);
	return device;
}

void _RenderDestroyDevice(VkDevice* _device) {
	if (_device) {
		vkDestroyDevice(*_device, VK_NULL_HANDLE);
	}
}

uint32_t _RenderGetPhysicalDeviceNumber(VkInstance* _instance) {
	MARS_RETURN_CLEAR;

	// Error check
	if (!_instance) {
		MARS_DEBUG_WARN("Invalid vulkan instance!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		return 0;
	}

	// Retrieve device info
	VkResult res;
	uint32_t physicalDeviceNumber = 0;
	if ((res = vkEnumeratePhysicalDevices(*_instance, &physicalDeviceNumber, VK_NULL_HANDLE)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error enumerating physical devices! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
		return 0;
	}
	return physicalDeviceNumber;
}

VkPhysicalDevice* _RenderGetPhysicalDevices(VkInstance* _instance, uint32_t _numDevices) {
	MARS_RETURN_CLEAR;
	VkPhysicalDevice* physicalDevices = NULL;

	// Error check
	if (!_instance) {
		MARS_DEBUG_WARN("Invalid vulkan instance!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		goto render_get_physical_devices_fail;
	}
	if (_numDevices == 0) {
		MARS_DEBUG_WARN("Number of specified devices must be greater than zero!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_PARAMETER);
		goto render_get_physical_devices_fail;
	}

	// Allocate buffers
	physicalDevices = MARS_MALLOC(_numDevices * sizeof(*physicalDevices));
	if (!physicalDevices) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate physical device array!");
		goto render_get_physical_devices_fail;
	}

	// Retrieve device info
	VkResult res;
	if ((res = vkEnumeratePhysicalDevices(*_instance, &_numDevices, physicalDevices)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error retrieving physical devices! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
		goto render_get_physical_devices_fail;
	}
	return physicalDevices;

render_get_physical_devices_fail:
	MARS_FREE(physicalDevices);
	return NULL;
}

void _RenderDestroyPhysicalDevices(VkPhysicalDevice** _physicalDevices) {
	MARS_FREE(*_physicalDevices);
}

uint32_t _RenderGetBestPhysicalDeviceIndex(VkPhysicalDevice* _physicalDevices, uint32_t _numDevices) {
	MARS_RETURN_CLEAR;
	VkPhysicalDeviceProperties* physicalDeviceProperties = NULL;
	VkPhysicalDeviceMemoryProperties* physicalDeviceMemoryProperties = NULL;
	uint32_t* discreteGPUIndices = NULL;
	uint32_t* integratedGPUIndices = NULL;

	// Error check
	if (!_physicalDevices) {
		MARS_DEBUG_WARN("Invalid physical device buffer!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		goto render_get_best_physical_device_index_fail;
	}
	if (_numDevices == 0) {
		MARS_DEBUG_WARN("Number of specified devices must be greater than zero!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_PARAMETER);
		goto render_get_best_physical_device_index_fail;
	}

	// Allocate buffers
	physicalDeviceProperties = MARS_MALLOC(_numDevices * sizeof(*physicalDeviceProperties));
	if (!physicalDeviceProperties) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate physical device property buffer!");
		goto render_get_best_physical_device_index_fail;
	}
	physicalDeviceMemoryProperties = MARS_MALLOC(_numDevices * sizeof(*physicalDeviceMemoryProperties));
	if (!physicalDeviceMemoryProperties) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate physical device memory property buffer!");
		goto render_get_best_physical_device_index_fail;
	}
	discreteGPUIndices = MARS_MALLOC(_numDevices * sizeof(*discreteGPUIndices));
	if (!discreteGPUIndices) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate discrete GPU index buffer!");
		goto render_get_best_physical_device_index_fail;
	}
	integratedGPUIndices = MARS_MALLOC(_numDevices * sizeof(*integratedGPUIndices));
	if (!integratedGPUIndices) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate integrated GPU index buffer!");
		goto render_get_best_physical_device_index_fail;
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
		if (bestPhysicalDeviceMemory < _RenderGetPhysicalDeviceTotalMemory(&physicalDeviceMemoryProperties[deviceIdx])) {
			bestPhysicalDeviceIdx = deviceIdx;
		}
	}
	for(uint32_t i=0; i<numIntegratedGPU; ++i) {
		uint32_t deviceIdx = integratedGPUIndices[i];
		if (bestPhysicalDeviceMemory < _RenderGetPhysicalDeviceTotalMemory(&physicalDeviceMemoryProperties[deviceIdx])) {
			bestPhysicalDeviceIdx = deviceIdx;
		}
	}
	return bestPhysicalDeviceIdx;

render_get_best_physical_device_index_fail:
	MARS_FREE(physicalDeviceProperties);
	MARS_FREE(physicalDeviceMemoryProperties);
	MARS_FREE(discreteGPUIndices);
	MARS_FREE(integratedGPUIndices);
	return 0;
}

uint32_t _RenderGetPhysicalDeviceTotalMemory(VkPhysicalDeviceMemoryProperties* _properties) {
	MARS_RETURN_CLEAR;

	// Error check
	if (!_properties) {
		MARS_DEBUG_WARN("Invalid physical device proeprty buffer!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
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

uint32_t _RenderGetQueueFamilyNumber(VkPhysicalDevice* _physicalDevice) {
	MARS_RETURN_CLEAR;

	// Error check
	if (!_physicalDevice) {
		MARS_DEBUG_WARN("Invalid physical device reference!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		return 0;
	}

	// Retrieve queue info
	uint32_t queueFamilyNumber = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(*_physicalDevice, &queueFamilyNumber, VK_NULL_HANDLE);
	return queueFamilyNumber;
}

VkQueueFamilyProperties* _RenderGetQueueFamilyProperties(VkPhysicalDevice* _physicalDevice, uint32_t _numQueueFamily) {
	MARS_RETURN_CLEAR;
	VkQueueFamilyProperties* queueFamilyProperties = NULL;

	// Error check
	if (!_physicalDevice) {
		MARS_DEBUG_WARN("Invalid physical device reference!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		goto render_get_queue_family_properties_fail;
	}
	if (_numQueueFamily == 0) {
		MARS_DEBUG_WARN("Number of specified devices must be greater than zero!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_PARAMETER);
		goto render_get_queue_family_properties_fail;
	}

	// Retrieve queue info
	queueFamilyProperties = MARS_MALLOC(_numQueueFamily * sizeof(*queueFamilyProperties));
	if (!queueFamilyProperties) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate queue family property buffer!");
		goto render_get_queue_family_properties_fail;
	}
	vkGetPhysicalDeviceQueueFamilyProperties(*_physicalDevice, &_numQueueFamily, queueFamilyProperties);
	return queueFamilyProperties;

render_get_queue_family_properties_fail:
	MARS_FREE(queueFamilyProperties);
	return NULL;
}

void _RenderDestroyQueueFamilyProperties(VkQueueFamilyProperties** _queueFamilyProperties) {
	if (_queueFamilyProperties) {
		MARS_FREE(*_queueFamilyProperties);
	}
}

uint32_t _RenderGetBestGraphicsQueueFamilyIndex(VkQueueFamilyProperties* _queueFamilyProperties, uint32_t _numQueueFamily) {
	MARS_RETURN_CLEAR;
	uint32_t bestGraphicsQueueFamilyCount = 0;
	uint32_t bestGraphicsQueueFamilyIdx = 0;
	uint32_t* graphicsQueueFamilyIndices = NULL;

	// Error check
	if (!_queueFamilyProperties) {
		MARS_DEBUG_WARN("Invalid queue family properties reference!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		goto render_get_best_graphics_queue_family_index_fail;
	}

	// Allocate index buffer
	graphicsQueueFamilyIndices = MARS_MALLOC(_numQueueFamily * sizeof(*graphicsQueueFamilyIndices));
	if (!graphicsQueueFamilyIndices) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate graphics queue family index buffer!");
		goto render_get_best_graphics_queue_family_index_fail;
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

render_get_best_graphics_queue_family_index_fail:
	MARS_FREE(graphicsQueueFamilyIndices);
	return bestGraphicsQueueFamilyIdx;
}

uint32_t _RenderGetGraphicsQueueMode(VkQueueFamilyProperties* _queueFamilyProperties, uint32_t _graphicsQueueFamiltyIdx) {
	MARS_RETURN_CLEAR;

	// Error check
	if (!_queueFamilyProperties) {
		MARS_DEBUG_WARN("Invalid queue family properties reference!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		return 2;
	}

	// Check properties
	uint32_t queueCount = _queueFamilyProperties[_graphicsQueueFamiltyIdx].queueCount;
	if (queueCount == 1) { return 0; }
	else if (queueCount > 1) { return 1; }
	else { return 2; }
}

VkQueue _RenderGetDrawingQueue(VkDevice* _device, uint32_t _graphicsQueueFamilyIdx) {
	MARS_RETURN_CLEAR;
	VkQueue drawingQueue = VK_NULL_HANDLE;

	// Error check
	if (!_device) {
		MARS_DEBUG_WARN("Invalid device reference!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		return drawingQueue;
	}

	// Retrieve queue
	vkGetDeviceQueue(*_device, _graphicsQueueFamilyIdx, 0, &drawingQueue);
	return drawingQueue;
}

VkQueue _RenderGetPresentingQueue(VkDevice* _device, uint32_t _graphicsQueueFamilyIdx, uint32_t _graphicsQueueMode) {
	MARS_RETURN_CLEAR;
	VkQueue presentingQueue = VK_NULL_HANDLE;

	// Error check
	if (!_device) {
		MARS_DEBUG_WARN("Invalid device reference!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_REFERENCE);
		return presentingQueue;
	}
	if (_graphicsQueueMode > 1) {
		MARS_DEBUG_WARN("Invalid graphics queue mode!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_PARAMETER);
		return presentingQueue;
	}

	// Retrieve queue
	vkGetDeviceQueue(*_device, _graphicsQueueFamilyIdx, _graphicsQueueMode, &presentingQueue);
	return presentingQueue;
}

VkSurfaceKHR _RenderCreateSurface(GLFWwindow* _window, VkInstance* _instance) {
	MARS_RETURN_CLEAR;
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkResult res;
	if ((res = glfwCreateWindowSurface(*_instance, _window, VK_NULL_HANDLE, &surface)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating window surface! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
	}
	return surface;
}

void _RenderDestroySurface(VkSurfaceKHR* _surface, VkInstance* _instance) {
	vkDestroySurfaceKHR(*_instance, *_surface, VK_NULL_HANDLE);
}

VkBool32 _RenderGetSurfaceSupport(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice, uint32_t _graphicsQueueFamilyIdx) {
	MARS_RETURN_CLEAR;
	VkBool32 surfaceSupport = 0;
	VkResult res;
	if ((res = vkGetPhysicalDeviceSurfaceSupportKHR(*_physicalDevice, _graphicsQueueFamilyIdx, *_surface, &surfaceSupport)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error getting surface support! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
	}
	return surfaceSupport;
}

VkSurfaceCapabilitiesKHR _RenderGetSurfaceCapabilities(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice) {
	MARS_RETURN_CLEAR;
	VkSurfaceCapabilitiesKHR surfaceCapabilities = { 0 };
	VkResult res;
	if ((res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*_physicalDevice, *_surface, &surfaceCapabilities)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error getting surface capabilities! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
	}
	return surfaceCapabilities;
}

VkSurfaceFormatKHR _RenderGetBestSurfaceFormat(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice) {
	MARS_RETURN;
	uint32_t numSurfaceFormat = 0;
	VkSurfaceFormatKHR* surfaceFormats = NULL;
	VkSurfaceFormatKHR bestSurfaceFormat = { 0 };

	// Get number of surface formats
	VkResult res;
	if ((res = vkGetPhysicalDeviceSurfaceFormatsKHR(*_physicalDevice, *_surface, &numSurfaceFormat, VK_NULL_HANDLE)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error enumerating surface formats! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
		goto render_get_best_surface_format_fail;
	}

	// Get all surface formats
	surfaceFormats = MARS_MALLOC(numSurfaceFormat * sizeof(*surfaceFormats));
	if (!surfaceFormats) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate surface formats buffer!");
		goto render_get_best_surface_format_fail;
	}
	if ((res = vkGetPhysicalDeviceSurfaceFormatsKHR(*_physicalDevice, *_surface, &numSurfaceFormat, surfaceFormats)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error retrieving surface formats! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
		goto render_get_best_surface_format_fail;
	}
	bestSurfaceFormat = surfaceFormats[0];

render_get_best_surface_format_fail:
	MARS_FREE(surfaceFormats);
	return bestSurfaceFormat;

}

VkPresentModeKHR _RenderGetBestPresentMode(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice) {
	MARS_RETURN_CLEAR;
	uint32_t numPresentMode = 0;
	VkPresentModeKHR* presentModes = NULL;
	VkPresentModeKHR bestPresentMode = VK_PRESENT_MODE_FIFO_KHR;
	VkResult res;

	// Get number of present modes
	if ((res = vkGetPhysicalDeviceSurfacePresentModesKHR(*_physicalDevice, *_surface, &numPresentMode, VK_NULL_HANDLE)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error enumerating present modes! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
		goto render_get_best_present_mode_fail;
	}

	// Get present modes
	presentModes = MARS_MALLOC(numPresentMode * sizeof(*presentModes));
	if (!presentModes) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate present modes buffer!");
		goto render_get_best_present_mode_fail;
	}
	if ((res = vkGetPhysicalDeviceSurfacePresentModesKHR(*_physicalDevice, *_surface, &numPresentMode, presentModes)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error retrieving present modes! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
		goto render_get_best_present_mode_fail;
	}

	// Select best present mode
	for(uint32_t i=0; i<numPresentMode; ++i) {
		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
			bestPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		}
	}

render_get_best_present_mode_fail:
	MARS_FREE(presentModes);
	return bestPresentMode;
}

VkExtent2D _RenderGetBestSwapchainExtent(VkSurfaceCapabilitiesKHR* _surfaceCapabilities, GLFWwindow* _window) {
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

VkSwapchainKHR _RenderCreateSwapchain(VkDevice* _device, VkSurfaceKHR* _surface, VkSurfaceCapabilitiesKHR* _surfaceCapabilities, VkSurfaceFormatKHR* _surfaceFormat, VkExtent2D* _swapChainExtent, VkPresentModeKHR* _presentMode, uint32_t _imageArrayLayers, uint32_t _graphicsQueueMode) {
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
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
	}
	return swapchain;
}

void _RenderDestroySwapchain(VkDevice* _device, VkSwapchainKHR* _swapchain) {
	vkDestroySwapchainKHR(*_device, *_swapchain, VK_NULL_HANDLE);
}

uint32_t _RenderGetSwapchainImageNumber(VkDevice* _device, VkSwapchainKHR* _swapchain) {
	MARS_RETURN_CLEAR;
	uint32_t numSwapchainImages;
	VkResult res;
	if ((res = vkGetSwapchainImagesKHR(*_device, *_swapchain, &numSwapchainImages, VK_NULL_HANDLE)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error enumerating swapchain images! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
		return 0;
	}
	return numSwapchainImages;
}

VkImage* _RenderGetSwapchainImages(VkDevice* _device, VkSwapchainKHR* _swapchain, uint32_t _numSwapchainImages) {
	MARS_RETURN_CLEAR;
	VkImage* swapchainImages = MARS_MALLOC(_numSwapchainImages * sizeof(*swapchainImages));
	if (!swapchainImages) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate swapchain images buffer!");
		return NULL;
	}
	VkResult res;
	if ((res = vkGetSwapchainImagesKHR(*_device, *_swapchain, &_numSwapchainImages, swapchainImages)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error populating swapchain images! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
		return NULL;
	}
	return swapchainImages;
}

void _RenderDestroySwapchainImages(VkImage** _images) {
	MARS_FREE(*_images);
}

VkImageView* _RenderCreateImageViews(VkDevice* _device, VkImage** _images, VkSurfaceFormatKHR* _format, uint32_t _numImages, uint32_t _imageArrayLayers) {
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
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate image view create info buffer!");
		goto render_create_image_views_fail;
	}
	imageViews = MARS_MALLOC(_numImages * sizeof(*imageViews));
	if (!imageViews) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate image view buffer!");
		goto render_create_image_views_fail;
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
			MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
			goto render_create_image_views_fail;
		}
	}

	MARS_FREE(imageViewCreateInfo);
	return imageViews;

render_create_image_views_fail:
	MARS_FREE(imageViewCreateInfo);
	MARS_FREE(imageViews);
	return NULL;
}

void _RenderDestroyImageViews(VkDevice* _device, VkImageView** _imageViews, uint32_t _numImageViews) {
	for(uint32_t i = 0; i < _numImageViews; ++i) {
		vkDestroyImageView(*_device, (*_imageViews)[i], VK_NULL_HANDLE);
	}
}

VkRenderPass _RenderCreateRenderPass(VkDevice* _device, VkSurfaceFormatKHR* _format) {
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
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
	}
	return renderPass;
}

void _RenderDestroyRenderPass(VkDevice* _device, VkRenderPass *_renderPass) {
	vkDestroyRenderPass(*_device, *_renderPass, VK_NULL_HANDLE);
}

VkFramebuffer* _RenderCreateFramebuffers(VkDevice *_device, VkRenderPass* _renderPass, VkExtent2D *_extent, VkImageView** _imageViews, uint32_t _numImageViews) {
	MARS_RETURN_CLEAR;
	VkFramebufferCreateInfo* framebufferCreateInfo = NULL;
	VkFramebuffer* framebuffers = NULL;

	// Allocate framebuffer arrays
	framebufferCreateInfo = MARS_MALLOC(_numImageViews * sizeof(*framebufferCreateInfo));
	if (!framebufferCreateInfo) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate framebuffer create info buffer!");
		goto render_create_framebuffers_fail;
	}
	framebuffers = MARS_MALLOC(_numImageViews * sizeof((framebuffers)));
	if (!framebuffers) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate framebuffer buffer!");
		goto render_create_framebuffers_fail;
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
			MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
			goto render_create_framebuffers_fail;
		}
	}

	MARS_FREE(framebufferCreateInfo);
	return framebuffers;

render_create_framebuffers_fail:
	MARS_FREE(framebuffers);
	MARS_FREE(framebufferCreateInfo);
	return NULL;
}

void _RenderDestroyFramebuffers(VkDevice* _device, VkFramebuffer** _framebuffers, uint32_t _numFrameBuffers) {
	for(uint32_t i = 0; i < _numFrameBuffers; ++i) {
		vkDestroyFramebuffer(*_device, (*_framebuffers)[i], VK_NULL_HANDLE);
	}
	MARS_FREE(*_framebuffers);
}

char* _RenderGetShaderCode(const char* _filename, uint32_t* _shaderSize) {
	MARS_RETURN_CLEAR;
	char* shaderCode = NULL;
	FILE* fp = NULL;

	// Validate inputs
	if (!_shaderSize) {
		MARS_DEBUG_WARN("File size destination pointer invalid!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_PARAMETER);
		goto render_get_shader_code_fail;
	}

	// Open shader file
	fp = fopen(_filename, "rb+");
	if (!fp) {
		MARS_DEBUG_WARN("Failed to open shader file! (%s)", _filename);
		MARS_RETURN_SET(MARS_RETURN_FILESYSTEM_FAILURE);
		goto render_get_shader_code_fail;
	}

	// Get file size
	fseek(fp, 0l, SEEK_END);
	*_shaderSize = (uint32_t)ftell(fp);
	rewind(fp);

	// Read shader code
	shaderCode = MARS_MALLOC((*_shaderSize) * sizeof(*shaderCode));
	if (!shaderCode) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate shader code buffer!");
		goto render_get_shader_code_fail;
	}
	fread(shaderCode, sizeof(*shaderCode), *_shaderSize, fp);

	fclose(fp);
	return shaderCode;
render_get_shader_code_fail:
	if (fp) fclose(fp);
	MARS_FREE(shaderCode);
	return NULL;
}

void _RenderDestroyShaderCode(char** _shaderCode) {
	MARS_FREE(*_shaderCode);
}

VkShaderModule _RenderCreateShaderModule(VkDevice* _device, char* _shaderCode, uint32_t _shaderSize) {
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
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
	}
	return shaderModule;
}

void _RenderDestroyShaderModule(VkDevice* _device, VkShaderModule* _shaderModule) {
	vkDestroyShaderModule(*_device, *_shaderModule, VK_NULL_HANDLE);
}

VkPipelineLayout _RenderCreatePipelineLayout(VkDevice* _device) {
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
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
	}
	return pipelineLayout;
}

void _RenderDestroyPipelineLayout(VkDevice* _device, VkPipelineLayout* _pipelineLayout) {
	vkDestroyPipelineLayout(*_device, *_pipelineLayout, VK_NULL_HANDLE);
}

VkPipelineShaderStageCreateInfo _RenderConfigureVertexShaderStageCreateInfo(VkShaderModule* _vertexShaderModule, const char* _entryName) {
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

VkPipelineShaderStageCreateInfo _RenderConfigureFragmentShaderStageCreateInfo(VkShaderModule* _fragmentShaderModule, const char* _entryName) {
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

VkPipelineVertexInputStateCreateInfo _RenderConfigureVertexInputStateCreateInfo() {
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

VkPipelineInputAssemblyStateCreateInfo _RenderConfigureInputAssemblyStateCreateInfo() {
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		VK_FALSE
	};

	return inputAssemblyStateCreateInfo;
}

VkPipeline _RenderCreateGraphicsPipeline(VkDevice* _device, VkPipelineLayout* _pipelineLayout, VkShaderModule *_vertexShaderModule, VkShaderModule* _fragmentShaderModule, VkRenderPass* _renderPass, VkExtent2D* _extent) {
	MARS_RETURN_CLEAR;

	char entryName[] = "main";
	VkPipelineShaderStageCreateInfo shaderStageCreateInfo[] = {
		_RenderConfigureVertexShaderStageCreateInfo(_vertexShaderModule, entryName),
		_RenderConfigureFragmentShaderStageCreateInfo(_fragmentShaderModule, entryName)
	};
	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = _RenderConfigureVertexInputStateCreateInfo();
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = _RenderConfigureInputAssemblyStateCreateInfo();
	VkViewport viewport = _RenderConfigureViewport(_extent);
	VkRect2D scissor = _RenderConfigureScissor(_extent, 0, 0, 0, 0);
	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = _RenderConfigureViewportStateCreateInfo(&viewport, &scissor);
	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = _RenderConfigureRasterizationStateCreateInfo();
	VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = _RenderConfigureMultisampleStateCreateInfo();
	VkPipelineColorBlendAttachmentState colorBlendAttachmentState = _RenderConfigureColorBlendAttachmentState();
	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = _RenderConfigureColorBlendStateCreateInfo(&colorBlendAttachmentState);

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
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
	}
	return graphicsPipeline;
}

void _RenderDestroyGraphicsPipeline(VkDevice* _device, VkPipeline* _graphicsPipeline) {
	vkDestroyPipeline(*_device, *_graphicsPipeline, VK_NULL_HANDLE);
}

VkViewport _RenderConfigureViewport(VkExtent2D *_extent) {
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

VkRect2D _RenderConfigureScissor(VkExtent2D *_extent, uint32_t _left, uint32_t _right, uint32_t _top, uint32_t _bottom) {
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

VkPipelineViewportStateCreateInfo _RenderConfigureViewportStateCreateInfo(VkViewport* _viewport, VkRect2D *_scissor) {
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

VkPipelineRasterizationStateCreateInfo _RenderConfigureRasterizationStateCreateInfo() {
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

VkPipelineMultisampleStateCreateInfo _RenderConfigureMultisampleStateCreateInfo() {
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

VkPipelineColorBlendAttachmentState _RenderConfigureColorBlendAttachmentState() {
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

VkPipelineColorBlendStateCreateInfo _RenderConfigureColorBlendStateCreateInfo(VkPipelineColorBlendAttachmentState* _colorBlendAttachmentState) {
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

VkCommandPool _RenderCreateCommandPool(VkDevice* _device, uint32_t _queueFamilyIndex) {
	MARS_RETURN_CLEAR;
	VkCommandPoolCreateInfo commandPoolCreateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		VK_NULL_HANDLE,
		0,
		_queueFamilyIndex
	};

	VkCommandPool commandPool;
	VkResult res;
	if ((res = vkCreateCommandPool(*_device, &commandPoolCreateInfo, VK_NULL_HANDLE, &commandPool)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating command pool! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
	}
	return commandPool;
}

void _RenderDestroyCommandPool(VkDevice* _device, VkCommandPool* _commandPool) {
	vkDestroyCommandPool(*_device, *_commandPool, VK_NULL_HANDLE);
}

VkCommandBuffer* _RenderCreateCommandBuffers(VkDevice* _device, VkCommandPool* _commandPool, uint32_t _numCommandBuffers) {
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
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate command buffers!");
	}
	VkResult res;
	if ((res = vkAllocateCommandBuffers(*_device, &commandBufferAllocateInfo, commandBuffers)) != VK_SUCCESS) {
		MARS_DEBUG_WARN("Vulkan error creating command buffers! (%d)", (int)res);
		MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
	}
	return commandBuffers;
}

void _RenderDestroyCommandBuffers(VkDevice* _device, VkCommandBuffer** _commandBuffers, VkCommandPool* _commandPool, uint32_t _numCommandBuffers) {
	vkFreeCommandBuffers(*_device, *_commandPool, _numCommandBuffers, *_commandBuffers);
	MARS_FREE(*_commandBuffers);
}

void _RenderRecordCommandBuffers(VkCommandBuffer** _commandBuffers, VkRenderPass* _renderPass, VkFramebuffer** _framebuffers, VkExtent2D* _extent, VkPipeline* _pipeline, uint32_t _numCommandBuffers) {
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
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate command buffer info buffer!");
		goto render_record_command_buffers_fail;
	}
	renderPassBeginInfos = MARS_MALLOC(_numCommandBuffers * sizeof(*renderPassBeginInfos));
	if (!renderPassBeginInfos) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate render pass info buffer!");
		goto render_record_command_buffers_fail;
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
			MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
			goto render_record_command_buffers_fail;
		}
		vkCmdBeginRenderPass((*_commandBuffers)[i], &renderPassBeginInfos[i], VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline((*_commandBuffers)[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *_pipeline);
		vkCmdDraw((*_commandBuffers)[i], 3, 1, 0, 0);
		vkCmdEndRenderPass((*_commandBuffers)[i]);
		if ((res = vkEndCommandBuffer((*_commandBuffers)[i])) != VK_SUCCESS) {
			MARS_DEBUG_WARN("Vulkan error ending command submission! (%d)", (int)res);
			MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
			goto render_record_command_buffers_fail;
		}
	}

render_record_command_buffers_fail:
	MARS_FREE(commandBufferBeginInfos);
	MARS_FREE(renderPassBeginInfos);
}

VkSemaphore* _RenderCreateSemaphores(VkDevice* _device, uint32_t _maxFrames) {
	MARS_RETURN_CLEAR;
	VkSemaphore* semaphores = NULL;

	VkSemaphoreCreateInfo semaphoreCreateInfo = {
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		VK_NULL_HANDLE,
		0
	};
	semaphores = MARS_MALLOC(_maxFrames * sizeof(*semaphores));
	if (!semaphores) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate semaphores buffer!");
		goto render_create_semaphores_fail;
	}
	for(uint32_t i = 0; i < _maxFrames; ++i) {
		VkResult res;
		if ((res = vkCreateSemaphore(*_device, &semaphoreCreateInfo, VK_NULL_HANDLE, &semaphores[i])) != VK_SUCCESS) {
			MARS_DEBUG_WARN("Vulkan error creating semaphore! (%d)", (int)res);
			MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
			goto render_create_semaphores_fail;
		}
	}
	return semaphores;

render_create_semaphores_fail:
	MARS_FREE(semaphores);
	return NULL;
}

void _RenderDestroySemaphores(VkDevice* _device, VkSemaphore** _semaphores, uint32_t _maxFrames) {
	for(uint32_t i = 0; i < _maxFrames; ++i) {
		vkDestroySemaphore(*_device, (*_semaphores)[i], VK_NULL_HANDLE);
	}
	MARS_FREE(*_semaphores);
}

VkFence* _RenderCreateFences(VkDevice* _device, uint32_t _maxFrames) {
	MARS_RETURN_CLEAR;
	VkFence* fences = NULL;

	VkFenceCreateInfo fenceCreateInfo = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		VK_NULL_HANDLE,
		VK_FENCE_CREATE_SIGNALED_BIT
	};
	fences = MARS_MALLOC(_maxFrames * sizeof(*fences));
	if (!fences) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate fences buffer!");
		goto render_create_fences_fail;
	}
	for(uint32_t i = 0; i < _maxFrames; ++i) {
		VkResult res;
		if ((res = vkCreateFence(*_device, &fenceCreateInfo, VK_NULL_HANDLE, &fences[i])) != VK_SUCCESS) {
			MARS_DEBUG_WARN("Vulkan error creating fence! (%d)", (int)res);
			MARS_RETURN_SET(MARS_RETURN_VULKAN_FAILURE);
			goto render_create_fences_fail;
		}
	}
	return fences;

render_create_fences_fail:
	MARS_FREE(fences);
	return NULL;
}

void _RenderDestroyFences(VkDevice* _device, VkFence** _fences, uint32_t _maxFrames) {
	for(uint32_t i = 0; i < _maxFrames; ++i) {
		vkDestroyFence(*_device, (*_fences)[i], VK_NULL_HANDLE);
	}
	MARS_FREE(*_fences);
}

VkFence* _RenderCreateEmptyFences(uint32_t _maxFrames) {
	VkFence* fences = MARS_MALLOC(_maxFrames * sizeof(*fences));
	if (!fences) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate fences buffer!");
		return NULL;
	}
	for(uint32_t i = 0; i < _maxFrames; ++i) {
		fences[i] = VK_NULL_HANDLE;
	}
	return fences;
}

void _RenderDestroyEmptyFences(VkFence** _fences) {
	MARS_FREE(*_fences);
}