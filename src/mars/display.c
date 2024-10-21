#include "mars/display.h"
#include "mars/renderer.h"
#include "mars/game.h"
#include "mars/shader.h"

#define MARS_PHYSICAL_DEVICE(w) ((w)->_physicalDevices[(w)->_physicalDeviceIdx])

Display* _CreateDisplay(DisplayDesc _desc) {
	MARS_RETURN_CLEAR;
	Display* display = NULL;
	char* vertexShaderCode = NULL;
	char* fragmentShaderCode = NULL;

	// Error check
	if (!_desc.name) {
		MARS_DEBUG_WARN("Invalid display name!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_PARAMETER);
		goto create_display_fail;
	}

	// Allocate display
	MARS_DEBUG_LOG("Allocating display");
	display = MARS_CALLOC(1, sizeof(*display));
	if (!display) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate display structure!");
		goto create_display_fail;
	}

	// Retrieve settings
	MARS_DEBUG_LOG("Retrieving display settings");
	uint32_t width = MARS_SETTINGS->_displaySettingsList->_width;
	uint32_t height = MARS_SETTINGS->_displaySettingsList->_height;

	// Initialize GLFW
	MARS_DEBUG_LOG("Initializing GLFW");
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	display->_window = glfwCreateWindow((int)width, (int)height, _desc.name, NULL, NULL);

	// Initialize Vulkan
	MARS_DEBUG_LOG("Initializing Vulkan");
	display->_instance = _RenderCreateInstance();
	if (MARS_RETURN != MARS_RETURN_OK) {
		MARS_DEBUG_WARN("Failed to initalize render backend!");
		goto create_display_fail;
	}

	// Create physical device
	MARS_DEBUG_LOG("Creating physical device");
	display->_numPhysicalDevices = _RenderGetPhysicalDeviceNumber(&display->_instance);
	display->_physicalDevices = _RenderGetPhysicalDevices(&display->_instance, display->_numPhysicalDevices);
	display->_physicalDeviceIdx = _RenderGetBestPhysicalDeviceIndex(display->_physicalDevices, display->_numPhysicalDevices);

	// Create render queue
	MARS_DEBUG_LOG("Creating render queue");
	uint32_t numQueueFamily = _RenderGetQueueFamilyNumber(&MARS_PHYSICAL_DEVICE(display));
	VkQueueFamilyProperties* queueFamilyProperties = _RenderGetQueueFamilyProperties(&MARS_PHYSICAL_DEVICE(display), numQueueFamily);
	display->_device = _RenderCreateDevice(&MARS_PHYSICAL_DEVICE(display), numQueueFamily, queueFamilyProperties);
	uint32_t bestGraphicsQueueFamilyIndex = _RenderGetBestGraphicsQueueFamilyIndex(queueFamilyProperties, numQueueFamily);
	display->_graphicsQueueMode = _RenderGetGraphicsQueueMode(queueFamilyProperties, bestGraphicsQueueFamilyIndex);
	display->_drawingQueue = _RenderGetDrawingQueue(&display->_device, bestGraphicsQueueFamilyIndex);
	display->_presentingQueue = _RenderGetPresentingQueue(&display->_device, bestGraphicsQueueFamilyIndex, display->_graphicsQueueMode);
	_RenderDestroyQueueFamilyProperties(&queueFamilyProperties);

	// Create surface
	MARS_DEBUG_LOG("Create surface");
	display->_surface = _RenderCreateSurface(display->_window, &display->_instance);
	VkBool32 surfaceSupported = _RenderGetSurfaceSupport(&display->_surface, &MARS_PHYSICAL_DEVICE(display), bestGraphicsQueueFamilyIndex);
	if (!surfaceSupported) {
		MARS_DEBUG_WARN("Failed to create swapchain!");
		goto create_display_fail;
	}
	
	// Create swapchain
	MARS_DEBUG_LOG("Creating swapchain");
	VkSurfaceCapabilitiesKHR surfaceCapabilities = _RenderGetSurfaceCapabilities(&display->_surface, &MARS_PHYSICAL_DEVICE(display));
	VkSurfaceFormatKHR bestSurfaceFormat = _RenderGetBestSurfaceFormat(&display->_surface, &MARS_PHYSICAL_DEVICE(display));
	VkPresentModeKHR bestPresentMode = _RenderGetBestPresentMode(&display->_surface, &MARS_PHYSICAL_DEVICE(display));
	VkExtent2D bestSwapchainExtent = _RenderGetBestSwapchainExtent(&surfaceCapabilities, display->_window);
	uint32_t imageArrayLayers = 1;
	display->_swapchain = _RenderCreateSwapchain(&display->_device, &display->_surface, &surfaceCapabilities, &bestSurfaceFormat, &bestSwapchainExtent, &bestPresentMode, imageArrayLayers, display->_graphicsQueueMode);
	display->_numSwapchainImages = _RenderGetSwapchainImageNumber(&display->_device, &display->_swapchain);
	display->_swapchainImages = _RenderGetSwapchainImages(&display->_device, &display->_swapchain, display->_numSwapchainImages);
	display->_swapchainImageViews = _RenderCreateImageViews(&display->_device, &display->_swapchainImages, &bestSurfaceFormat, display->_numSwapchainImages, imageArrayLayers);

	// Create framebuffers
	MARS_DEBUG_LOG("Creating framebuffer");
	display->_renderPass = _RenderCreateRenderPass(&display->_device, &bestSurfaceFormat);
	display->_framebuffers = _RenderCreateFramebuffers(&display->_device, &display->_renderPass, &bestSwapchainExtent, &display->_swapchainImageViews, display->_numSwapchainImages);

	// Create shader pipelines
	MARS_DEBUG_LOG("Creating default vertex shader module");
	size_t vertexShaderSize = base64Decode(_SHADER_BIN_DEFAULT_VERT_SPV, NULL, 0);
	MARS_DEBUG_LOG("Vertex shader binary size: %d", vertexShaderSize);
	vertexShaderCode = MARS_MALLOC(vertexShaderSize);
	if (!vertexShaderCode) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate default vertex shader code buffer!");
		goto create_display_fail;
	}
	if (base64Decode(_SHADER_BIN_DEFAULT_VERT_SPV, vertexShaderCode, vertexShaderSize) != vertexShaderSize) {
		MARS_DEBUG_WARN("Failed to decode vertex shader code!");
		MARS_RETURN_SET(MARS_RETURN_GENERIC_ERROR);
		goto create_display_fail;
	}
	VkShaderModule vertexShaderModule = _RenderCreateShaderModule(&display->_device, vertexShaderCode, (uint32_t)vertexShaderSize);
	MARS_FREE(vertexShaderCode);
	vertexShaderCode = NULL;

	MARS_DEBUG_LOG("Creating default fragment shader module");
	size_t fragmentShaderSize = base64Decode(_SHADER_BIN_DEFAULT_FRAG_SPV, NULL, 0);
	fragmentShaderCode = MARS_MALLOC(fragmentShaderSize);
	if (!fragmentShaderCode) {
		MARS_ABORT(MARS_ERROR_STATUS_BAD_ALLOC, "Failed to allocate default fragment shader code buffer!");
		goto create_display_fail;
	}
	if (base64Decode(_SHADER_BIN_DEFAULT_FRAG_SPV, fragmentShaderCode, fragmentShaderSize) != fragmentShaderSize) {
		MARS_DEBUG_WARN("Failed to decode fragment shader code!");
		MARS_RETURN_SET(MARS_RETURN_GENERIC_ERROR);
		goto create_display_fail;
	}
	VkShaderModule fragmentShaderModule = _RenderCreateShaderModule(&display->_device, fragmentShaderCode, (uint32_t)fragmentShaderSize);
	MARS_FREE(fragmentShaderCode);
	fragmentShaderCode = NULL;

	MARS_DEBUG_LOG("Creating default shader pipeline");
	display->_pipelineLayout = _RenderCreatePipelineLayout(&display->_device);
	display->_graphicsPipeline = _RenderCreateGraphicsPipeline(&display->_device, &display->_pipelineLayout, &vertexShaderModule, &fragmentShaderModule, &display->_renderPass, &bestSwapchainExtent);
	_RenderDestroyShaderModule(&display->_device, &fragmentShaderModule);
	_RenderDestroyShaderCode(&fragmentShaderCode);
	_RenderDestroyShaderModule(&display->_device, &vertexShaderModule);
	_RenderDestroyShaderCode(&vertexShaderCode);

	// Create command buffers
	MARS_DEBUG_LOG("Creating command buffers");
	display->_commandPool = _RenderCreateCommandPool(&display->_device, bestGraphicsQueueFamilyIndex);
	display->_commandBuffers = _RenderCreateCommandBuffers(&display->_device, &display->_commandPool, display->_numSwapchainImages);
	_RenderRecordCommandBuffers(&display->_commandBuffers, &display->_renderPass, &display->_framebuffers, &bestSwapchainExtent, &display->_graphicsPipeline, display->_numSwapchainImages);

	// Create semaphores
	MARS_DEBUG_LOG("Creating semaphores");
	display->_maxFrames = 2;
	display->_waitSemaphores = _RenderCreateSemaphores(&display->_device, display->_maxFrames);
	display->_signalSemaphores = _RenderCreateSemaphores(&display->_device, display->_maxFrames);
	display->_frontFences = _RenderCreateFences(&display->_device, display->_maxFrames);
	display->_backFences = _RenderCreateEmptyFences(display->_numSwapchainImages);

	return display;
create_display_fail:
	MARS_FREE(vertexShaderCode);
	MARS_FREE(fragmentShaderCode);
	_DestroyDisplay(display);
	return NULL;
}

void _DestroyDisplay(Display* _display) {
	if (_display) {
		vkDeviceWaitIdle(_display->_device);
		_RenderDestroyEmptyFences(&_display->_backFences);
		_RenderDestroyFences(&_display->_device, &_display->_frontFences, _display->_maxFrames);
		_RenderDestroySemaphores(&_display->_device, &_display->_signalSemaphores, _display->_maxFrames);
		_RenderDestroySemaphores(&_display->_device, &_display->_waitSemaphores, _display->_maxFrames);
		_RenderDestroyCommandBuffers(&_display->_device, &_display->_commandBuffers, &_display->_commandPool, _display->_numSwapchainImages);
		_RenderDestroyCommandPool(&_display->_device, &_display->_commandPool);
		_RenderDestroyGraphicsPipeline(&_display->_device, &_display->_graphicsPipeline);
		_RenderDestroyPipelineLayout(&_display->_device, &_display->_pipelineLayout);
		_RenderDestroyFramebuffers(&_display->_device, &_display->_framebuffers, _display->_numSwapchainImages);
		_RenderDestroyRenderPass(&_display->_device, &_display->_renderPass);
		_RenderDestroyImageViews(&_display->_device, &_display->_swapchainImageViews, _display->_numSwapchainImages);
		_RenderDestroySwapchainImages(&_display->_swapchainImages);
		_RenderDestroySwapchain(&_display->_device, &_display->_swapchain);
		_RenderDestroySurface(&_display->_surface, &_display->_instance);
		_RenderDestroyDevice(&_display->_device);
		_RenderDestroyPhysicalDevices(&_display->_physicalDevices);
		_RenderDestroyInstance(&_display->_instance);
		glfwDestroyWindow(_display->_window);
		glfwTerminate();
		MARS_FREE(_display);
	}
}

void _UpdateDisplay(Display* _display) {
	glfwPollEvents();

	// Wait to acquire next image from the swapchain
	static uint32_t currentFrame = 0;
	vkWaitForFences(_display->_device, 1, &_display->_frontFences[currentFrame], VK_TRUE, UINT64_MAX);
	uint32_t imageIndex = 0;
	vkAcquireNextImageKHR(_display->_device, _display->_swapchain, UINT64_MAX, _display->_waitSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
	if (_display->_backFences[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(_display->_device, 1, &_display->_backFences[imageIndex], VK_TRUE, UINT64_MAX);
	}
	_display->_backFences[imageIndex] = _display->_frontFences[currentFrame];

	// Submit commands to draw queue
	VkPipelineStageFlags pipelineStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,
		VK_NULL_HANDLE,
		1,
		&_display->_waitSemaphores[currentFrame],
		&pipelineStage,
		1,
		&_display->_commandBuffers[imageIndex],
		1,
		&_display->_signalSemaphores[currentFrame]
	};
	vkResetFences(_display->_device, 1, &_display->_frontFences[currentFrame]);
	vkQueueSubmit(_display->_drawingQueue, 1, &submitInfo, _display->_frontFences[currentFrame]);

	// Present next image
	VkPresentInfoKHR presentInfo = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		VK_NULL_HANDLE,
		1,
		&_display->_signalSemaphores[currentFrame],
		1,
		&_display->_swapchain,
		&imageIndex,
		VK_NULL_HANDLE
	};
	vkQueuePresentKHR(_display->_presentingQueue, &presentInfo);
	currentFrame = (currentFrame + 1) % _display->_maxFrames;
}

bool _DisplayShouldClose(Display* _display) {
	if (!_display) {
		return true;
	}
	return glfwWindowShouldClose(_display->_window);
}