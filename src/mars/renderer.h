#ifndef MARS_RENDERER_H
#define MARS_RENDERER_H
/**
 * renderer.h
 * Declarations for Vulkan helper functions.
 */
#include "mars/common.h"


//----------------------------------------------------------------------------------
// Vulkan instance
//----------------------------------------------------------------------------------

VkInstance _RenderCreateInstance();

void _RenderDestroyInstance(VkInstance* _instance);


//----------------------------------------------------------------------------------
// Vulkan device
//----------------------------------------------------------------------------------

VkDevice _RenderCreateDevice(VkPhysicalDevice* _physicalDevice, uint32_t _numQueueFamily, VkQueueFamilyProperties* _queueFamilyProperties);

void _RenderDestroyDevice(VkDevice* _device);


//----------------------------------------------------------------------------------
// Physical devices
//----------------------------------------------------------------------------------

uint32_t _RenderGetPhysicalDeviceNumber(VkInstance* _instance);

VkPhysicalDevice* _RenderGetPhysicalDevices(VkInstance* _instance, uint32_t _numDevices);

void _RenderDestroyPhysicalDevices(VkPhysicalDevice** _physicalDevices);

uint32_t _RenderGetBestPhysicalDeviceIndex(VkPhysicalDevice* _physicalDevices, uint32_t _numDevices);

uint32_t _RenderGetPhysicalDeviceTotalMemory(VkPhysicalDeviceMemoryProperties* _properties);


//----------------------------------------------------------------------------------
// Render queues
//----------------------------------------------------------------------------------

uint32_t _RenderGetQueueFamilyNumber(VkPhysicalDevice* _physicalDevice);

VkQueueFamilyProperties* _RenderGetQueueFamilyProperties(VkPhysicalDevice* _physicalDevice, uint32_t _numQueueFamily);

void _RenderDestroyQueueFamilyProperties(VkQueueFamilyProperties** _queueFamilyProperties);

uint32_t _RenderGetBestGraphicsQueueFamilyIndex(VkQueueFamilyProperties* _queueFamilyProperties, uint32_t _numQueueFamily);

uint32_t _RenderGetGraphicsQueueMode(VkQueueFamilyProperties* _queueFamilyProperties, uint32_t _graphicsQueueFamiltyIdx);

VkQueue _RenderGetDrawingQueue(VkDevice* _device, uint32_t _graphicsQueueFamilyIdx);

VkQueue _RenderGetPresentingQueue(VkDevice* _device, uint32_t _graphicsQueueFamilyIdx, uint32_t _graphicsQueueMode);


//----------------------------------------------------------------------------------
// Surfaces & swapchains
//----------------------------------------------------------------------------------

VkSurfaceKHR _RenderCreateSurface(GLFWwindow* _window, VkInstance* _instance);

void _RenderDestroySurface(VkSurfaceKHR* _surface, VkInstance* _instance);

VkBool32 _RenderGetSurfaceSupport(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice, uint32_t _graphicsQueueFamilyIdx);

VkSurfaceCapabilitiesKHR _RenderGetSurfaceCapabilities(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice);

VkSurfaceFormatKHR _RenderGetBestSurfaceFormat(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice);

VkPresentModeKHR _RenderGetBestPresentMode(VkSurfaceKHR* _surface, VkPhysicalDevice* _physicalDevice);

VkExtent2D _RenderGetBestSwapchainExtent(VkSurfaceCapabilitiesKHR* _surfaceCapabilities, GLFWwindow* _window);

VkSwapchainKHR _RenderCreateSwapchain(VkDevice* _device, VkSurfaceKHR* _surface, VkSurfaceCapabilitiesKHR* _surfaceCapabilities, VkSurfaceFormatKHR* _surfaceFormat, VkExtent2D* _swapChainExtent, VkPresentModeKHR* _presentMode, uint32_t _imageArrayLayers, uint32_t _graphicsQueueMode);

void _RenderDestroySwapchain(VkDevice* _device, VkSwapchainKHR* _swapchain);

uint32_t _RenderGetSwapchainImageNumber(VkDevice* _device, VkSwapchainKHR* _swapchain);

VkImage* _RenderGetSwapchainImages(VkDevice* _device, VkSwapchainKHR* _swapchain, uint32_t _numSwapchainImages);

void _RenderDestroySwapchainImages(VkImage** _images);

VkImageView* _RenderCreateImageViews(VkDevice* _device, VkImage** _images, VkSurfaceFormatKHR* _format, uint32_t _imageNumber, uint32_t _imageArrayLayers);

void _RenderDestroyImageViews(VkDevice* _device, VkImageView** _imageViews, uint32_t _numImageViews);


//----------------------------------------------------------------------------------
// Framebuffer
//----------------------------------------------------------------------------------

VkRenderPass _RenderCreateRenderPass(VkDevice* _device, VkSurfaceFormatKHR* _format);

void _RenderDestroyRenderPass(VkDevice* _device, VkRenderPass *_renderPass);

VkFramebuffer* _RenderCreateFramebuffers(VkDevice *_device, VkRenderPass* _renderPass, VkExtent2D *_extent, VkImageView** _imageViews, uint32_t _numImageViews);

void _RenderDestroyFramebuffers(VkDevice* _device, VkFramebuffer** _framebuffers, uint32_t _numFrameBuffers);


//----------------------------------------------------------------------------------
// Shaders & pipelines
//----------------------------------------------------------------------------------

char* _RenderGetShaderCode(const char* _filename, uint32_t* _shaderSize);

void _RenderDestroyShaderCode(char** _shaderCode);

VkShaderModule _RenderCreateShaderModule(VkDevice* _device, char* _shaderCode, uint32_t _shaderSize);

void _RenderDestroyShaderModule(VkDevice* _device, VkShaderModule* _shaderModule);

VkPipelineLayout _RenderCreatePipelineLayout(VkDevice* _device);

void _RenderDestroyPipelineLayout(VkDevice* _device, VkPipelineLayout* _pipelineLayout);

VkPipelineShaderStageCreateInfo _RenderConfigureVertexShaderStageCreateInfo(VkShaderModule* _vertexShaderModule, const char* _entryName);

VkPipelineShaderStageCreateInfo _RenderConfigureFragmentShaderStageCreateInfo(VkShaderModule* _fragmentShaderModule, const char* _entryName);

VkPipelineVertexInputStateCreateInfo _RenderConfigureVertexInputStateCreateInfo();

VkPipelineInputAssemblyStateCreateInfo _RenderConfigureInputAssemblyStateCreateInfo();

VkPipeline _RenderCreateGraphicsPipeline(VkDevice* _device, VkPipelineLayout* _pipelineLayout, VkShaderModule *_vertexShaderModule, VkShaderModule* _fragmentShaderModule, VkRenderPass* _renderPass, VkExtent2D* _extent);

void _RenderDestroyGraphicsPipeline(VkDevice* _device, VkPipeline* _graphicsPipeline);


//----------------------------------------------------------------------------------
// Viewports
//----------------------------------------------------------------------------------

VkViewport _RenderConfigureViewport(VkExtent2D *_extent);

VkRect2D _RenderConfigureScissor(VkExtent2D *_extent, uint32_t _left, uint32_t _right, uint32_t _top, uint32_t _bottom);

VkPipelineViewportStateCreateInfo _RenderConfigureViewportStateCreateInfo(VkViewport* _viewport, VkRect2D *_scissor);

VkPipelineRasterizationStateCreateInfo _RenderConfigureRasterizationStateCreateInfo();

VkPipelineMultisampleStateCreateInfo _RenderConfigureMultisampleStateCreateInfo();

VkPipelineColorBlendAttachmentState _RenderConfigureColorBlendAttachmentState();

VkPipelineColorBlendStateCreateInfo _RenderConfigureColorBlendStateCreateInfo(VkPipelineColorBlendAttachmentState* _colorBlendAttachmentState);


//----------------------------------------------------------------------------------
// Command buffers
//----------------------------------------------------------------------------------

VkCommandPool _RenderCreateCommandPool(VkDevice* _device, uint32_t _queueFamilyIndex);

void _RenderDestroyCommandPool(VkDevice* _device, VkCommandPool* _commandPool);

VkCommandBuffer* _RenderCreateCommandBuffers(VkDevice* _device, VkCommandPool* _commandPool, uint32_t _numCommandBuffers);

void _RenderDestroyCommandBuffers(VkDevice* _device, VkCommandBuffer** _commandBuffers, VkCommandPool* _commandPool, uint32_t _numCommandBuffers);

void _RenderRecordCommandBuffers(VkCommandBuffer** _commandBuffers, VkRenderPass* _renderPass, VkFramebuffer** _framebuffers, VkExtent2D* _extent, VkPipeline* _pipeline, uint32_t _numCommandBuffers);


//----------------------------------------------------------------------------------
// Semaphores
//----------------------------------------------------------------------------------

VkSemaphore* _RenderCreateSemaphores(VkDevice* _device, uint32_t _maxFrames);

void _RenderDestroySemaphores(VkDevice* _device, VkSemaphore** _semaphores, uint32_t _maxFrames);

VkFence* _RenderCreateFences(VkDevice* _device, uint32_t _maxFrames);

void _RenderDestroyFences(VkDevice* _device, VkFence** _fences, uint32_t _maxFrames);

VkFence* _RenderCreateEmptyFences(uint32_t _maxFrames);

void _RenderDestroyEmptyFences(VkFence** _fences);

#endif // MARS_RENDERER_H