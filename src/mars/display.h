#ifndef MARS_DISPLAY_H
#define MARS_DISPLAY_H
/**
 * display.h
 * Display related functions & settings.
 */
#include "mars/common.h"

/// @brief List of all video related settings.
typedef struct {
	uint32_t _width;
	uint32_t _height;
	bool _fullscreen;
	bool _vsync;
} DisplaySettingsList;

/// @brief Top level game rendering structure.
typedef struct {
	GLFWwindow* _window;
	VkPhysicalDevice* _physicalDevices;
	VkImage* _swapchainImages;
	VkImageView* _swapchainImageViews;
	VkFramebuffer* _framebuffers;
	VkCommandBuffer* _commandBuffers;
	VkSemaphore* _waitSemaphores;
	VkSemaphore* _signalSemaphores;
	VkFence* _frontFences;
	VkFence* _backFences;
	VkInstance _instance;
	VkDevice _device;
	VkSurfaceKHR _surface;
	VkSwapchainKHR _swapchain;
	VkQueue _drawingQueue;
	VkQueue _presentingQueue;
	VkRenderPass _renderPass;
	VkPipelineLayout _pipelineLayout;
	VkPipeline _graphicsPipeline;
	VkCommandPool _commandPool;
	uint32_t _physicalDeviceIdx;
	uint32_t _numPhysicalDevices;
	uint32_t _graphicsQueueMode;
	uint32_t _numSwapchainImages;
	uint32_t _maxFrames;
} Display;

/// @brief Descriptor for creating the renderer
typedef struct {
	const char* name;		// Display title
} DisplayDesc;

Display* _CreateDisplay(DisplayDesc _desc);

void _DestroyDisplay(Display* _window);

void _UpdateDisplay(Display* _window);

bool _DisplayShouldClose(Display* _window);

#endif // MARS_DISPLAY_H