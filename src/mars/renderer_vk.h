#ifndef MARS_RENDERER_H
#define MARS_RENDERER_H
/**
 * renderer.h
 * Declarations for Vulkan helper functions.
 */
#include "mars/common.h"

/// @brief Container for vulkan renderer state.
typedef struct {
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
} RendererVulkan;

RendererVulkan* _RendererVKCreate(GLFWwindow* _window);

void _RendererVKDestroy(RendererVulkan* _renderer);

void _RendererVKUpdate(RendererVulkan* _renderer);


//----------------------------------------------------------------------------------
// Vulkan instance
//----------------------------------------------------------------------------------

VkInstance _RendererVKCreateInstance();

void _RendererVKDestroyInstance(VkInstance* _instance);


//----------------------------------------------------------------------------------
// Vulkan device
//----------------------------------------------------------------------------------

VkDevice _RendererVKCreateDevice(VkPhysicalDevice* _physicalDevice, uint32_t _numQueueFamily, VkQueueFamilyProperties* _queueFamilyProperties);

void _RendererVKDestroyDevice(VkDevice* _device);


//----------------------------------------------------------------------------------
// Physical devices
//----------------------------------------------------------------------------------

uint32_t _RendererVKGetPhysicalDeviceNumber(VkInstance* _instance);

VkPhysicalDevice* _RendererVKGetPhysicalDevices(VkInstance* _instance, uint32_t _numDevices);

void _RendererVKDestroyPhysicalDevices(VkPhysicalDevice** _physicalDevices);

uint32_t _RendererVKGetBestPhysicalDeviceIndex(VkPhysicalDevice* _physicalDevices, uint32_t _numDevices);

uint32_t _RendererVKGetPhysicalDeviceTotalMemory(VkPhysicalDeviceMemoryProperties* _properties);


//----------------------------------------------------------------------------------
// Render queues
//----------------------------------------------------------------------------------

uint32_t _RendererVKGetQueueFamilyNumber(VkPhysicalDevice* _physicalDevice);

VkQueueFamilyProperties* _RendererVKGetQueueFamilyProperties(VkPhysicalDevice* _physicalDevice, uint32_t _numQueueFamily);

void _RendererVKDestroyQueueFamilyProperties(VkQueueFamilyProperties** _queueFamilyProperties);

uint32_t _RendererVKGetBestGraphicsQueueFamilyIndex(VkQueueFamilyProperties* _queueFamilyProperties, uint32_t _numQueueFamily);

uint32_t _RendererVKGetGraphicsQueueMode(VkQueueFamilyProperties* _queueFamilyProperties, uint32_t _graphicsQueueFamiltyIdx);

VkQueue _RendererVKGetDrawingQueue(VkDevice* _device, uint32_t _graphicsQueueFamilyIdx);

VkQueue _RendererVKGetPresentingQueue(VkDevice* _device, uint32_t _graphicsQueueFamilyIdx, uint32_t _graphicsQueueMode);


//----------------------------------------------------------------------------------
// Surfaces & swapchains
//----------------------------------------------------------------------------------

VkSurfaceKHR _RendererVKCreateSurface(GLFWwindow* _window, VkInstance* _instance);

void _RendererVKDestroySurface(VkSurfaceKHR* _surface, VkInstance* _instance);

VkBool32 _RendererVKGetSurfaceSupport(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice, uint32_t _graphicsQueueFamilyIdx);

VkSurfaceCapabilitiesKHR _RendererVKGetSurfaceCapabilities(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice);

VkSurfaceFormatKHR _RendererVKGetBestSurfaceFormat(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice);

VkPresentModeKHR _RendererVKGetBestPresentMode(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice);

VkExtent2D _RendererVKGetBestSwapchainExtent(VkSurfaceCapabilitiesKHR* _surfaceCapabilities, GLFWwindow* _window);

VkSwapchainKHR _RendererVKCreateSwapchain(VkDevice* _device, VkSurfaceKHR* _surface, VkSurfaceCapabilitiesKHR* _surfaceCapabilities, VkSurfaceFormatKHR* _surfaceFormat, VkExtent2D* _swapChainExtent, VkPresentModeKHR* _presentMode, uint32_t _imageArrayLayers, uint32_t _graphicsQueueMode);

void _RendererVKDestroySwapchain(VkDevice* _device, VkSwapchainKHR* _swapchain);

uint32_t _RendererVKGetSwapchainImageNumber(VkDevice* _device, VkSwapchainKHR* _swapchain);

VkImage* _RendererVKGetSwapchainImages(VkDevice* _device, VkSwapchainKHR* _swapchain, uint32_t _numSwapchainImages);

void _RendererVKDestroySwapchainImages(VkImage** _images);

VkImageView* _RendererVKCreateImageViews(VkDevice* _device, VkImage** _images, VkSurfaceFormatKHR* _format, uint32_t _imageNumber, uint32_t _imageArrayLayers);

void _RendererVKDestroyImageViews(VkDevice* _device, VkImageView** _imageViews, uint32_t _numImageViews);


//----------------------------------------------------------------------------------
// Framebuffer
//----------------------------------------------------------------------------------

VkRenderPass _RendererVKCreateRenderPass(VkDevice* _device, VkSurfaceFormatKHR* _format);

void _RendererVKDestroyRenderPass(VkDevice* _device, VkRenderPass *_renderPass);

VkFramebuffer* _RendererVKCreateFramebuffers(VkDevice *_device, VkRenderPass* _renderPass, VkExtent2D *_extent, VkImageView** _imageViews, uint32_t _numImageViews);

void _RendererVKDestroyFramebuffers(VkDevice* _device, VkFramebuffer** _framebuffers, uint32_t _numFrameBuffers);


//----------------------------------------------------------------------------------
// Shaders & pipelines
//----------------------------------------------------------------------------------

char* _RendererVKGetShaderCode(const char* _filename, uint32_t* _shaderSize);

void _RendererVKDestroyShaderCode(char** _shaderCode);

VkShaderModule _RendererVKCreateShaderModule(VkDevice* _device, char* _shaderCode, uint32_t _shaderSize);

void _RendererVKDestroyShaderModule(VkDevice* _device, VkShaderModule* _shaderModule);

VkPipelineLayout _RendererVKCreatePipelineLayout(VkDevice* _device);

void _RendererVKDestroyPipelineLayout(VkDevice* _device, VkPipelineLayout* _pipelineLayout);

VkPipelineShaderStageCreateInfo _RendererVKConfigureVertexShaderStageCreateInfo(VkShaderModule* _vertexShaderModule, const char* _entryName);

VkPipelineShaderStageCreateInfo _RendererVKConfigureFragmentShaderStageCreateInfo(VkShaderModule* _fragmentShaderModule, const char* _entryName);

VkPipelineVertexInputStateCreateInfo _RendererVKConfigureVertexInputStateCreateInfo();

VkPipelineInputAssemblyStateCreateInfo _RendererVKConfigureInputAssemblyStateCreateInfo();

VkPipeline _RendererVKCreateGraphicsPipeline(VkDevice* _device, VkPipelineLayout* _pipelineLayout, VkShaderModule *_vertexShaderModule, VkShaderModule* _fragmentShaderModule, VkRenderPass* _renderPass, VkExtent2D* _extent);

void _RendererVKDestroyGraphicsPipeline(VkDevice* _device, VkPipeline* _graphicsPipeline);


//----------------------------------------------------------------------------------
// Viewports
//----------------------------------------------------------------------------------

VkViewport _RendererVKConfigureViewport(VkExtent2D *_extent);

VkRect2D _RendererVKConfigureScissor(VkExtent2D *_extent, uint32_t _left, uint32_t _right, uint32_t _top, uint32_t _bottom);

VkPipelineViewportStateCreateInfo _RendererVKConfigureViewportStateCreateInfo(VkViewport* _viewport, VkRect2D *_scissor);

VkPipelineRasterizationStateCreateInfo _RendererVKConfigureRasterizationStateCreateInfo();

VkPipelineMultisampleStateCreateInfo _RendererVKConfigureMultisampleStateCreateInfo();

VkPipelineColorBlendAttachmentState _RendererVKConfigureColorBlendAttachmentState();

VkPipelineColorBlendStateCreateInfo _RendererVKConfigureColorBlendStateCreateInfo(VkPipelineColorBlendAttachmentState* _colorBlendAttachmentState);


//----------------------------------------------------------------------------------
// Command buffers
//----------------------------------------------------------------------------------

VkCommandPool _RendererVKCreateCommandPool(VkDevice* _device, uint32_t _queueFamilyIndex);

void _RendererVKDestroyCommandPool(VkDevice* _device, VkCommandPool* _commandPool);

VkCommandBuffer* _RendererVKCreateCommandBuffers(VkDevice* _device, VkCommandPool* _commandPool, uint32_t _numCommandBuffers);

void _RendererVKDestroyCommandBuffers(VkDevice* _device, VkCommandBuffer** _commandBuffers, VkCommandPool* _commandPool, uint32_t _numCommandBuffers);

void _RendererVKRecordCommandBuffers(VkCommandBuffer** _commandBuffers, VkRenderPass* _renderPass, VkFramebuffer** _framebuffers, VkExtent2D* _extent, VkPipeline* _pipeline, uint32_t _numCommandBuffers);


//----------------------------------------------------------------------------------
// Semaphores
//----------------------------------------------------------------------------------

VkSemaphore* _RendererVKCreateSemaphores(VkDevice* _device, uint32_t _maxFrames);

void _RendererVKDestroySemaphores(VkDevice* _device, VkSemaphore** _semaphores, uint32_t _maxFrames);

VkFence* _RendererVKCreateFences(VkDevice* _device, uint32_t _maxFrames);

void _RendererVKDestroyFences(VkDevice* _device, VkFence** _fences, uint32_t _maxFrames);

VkFence* _RendererVKCreateEmptyFences(uint32_t _maxFrames);

void _RendererVKDestroyEmptyFences(VkFence** _fences);

#endif // MARS_RENDERER_H