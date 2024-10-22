#include "mars/display.h"
#include "mars/renderer_vk.h"
#include "mars/game.h"

Display* _CreateDisplay(DisplayDesc _desc) {
	MARS_RETURN_CLEAR;
	Display* display = NULL;

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

	// Initialize renderer
	int backend = _GetRendererBackend();
	if (backend < 0) {
		MARS_DEBUG_WARN("Failed to determine renderer backend!");
		MARS_RETURN_SET(MARS_RETURN_INVALID_PARAMETER);
		goto create_display_fail;
	}
	else switch(backend) {
		case MARS_RENDERER_BACKEND_VULKAN: 
			display->_renderer = _RendererVKCreate(display->_window); 
		break;
	}
	if (!display->_renderer) {
		MARS_DEBUG_WARN("Failed to initialize renderer!");
		goto create_display_fail;
	}

	return display;	
create_display_fail:
	_DestroyDisplay(display);
	return NULL;
}

void _DestroyDisplay(Display* _display) {
	if (_display) {
		int backend = _GetRendererBackend();
		switch(backend) {
			case MARS_RENDERER_BACKEND_VULKAN: 
				_RendererVKDestroy((RendererVulkan*)_display->_renderer);
			break;
		}
		glfwDestroyWindow(_display->_window);
		glfwTerminate();
		MARS_FREE(_display);
	}
}

void _UpdateDisplay(Display* _display) {
	glfwPollEvents();
	int backend = _GetRendererBackend();
	switch(backend) {
		case MARS_RENDERER_BACKEND_VULKAN: 
			_RendererVKUpdate((RendererVulkan*)_display->_renderer);
		break;
	}
	_RendererVKUpdate(_display->_renderer);
}

bool _DisplayShouldClose(Display* _display) {
	if (!_display) {
		return true;
	}
	return glfwWindowShouldClose(_display->_window);
}

int _GetRendererBackend() {
	// Error check
	if (!MARS_GAME || !MARS_SETTINGS || !MARS_SETTINGS->_displaySettingsList) { return -1; }

	// Determine default based on platform
	int backend = MARS_SETTINGS->_displaySettingsList->_rendererBackend;
	if (backend == MARS_RENDERER_BACKEND_DEFAULT) {
		#if defined(MARS_OS_WINDOWS) && !defined(MARS_DISABLE_RENDERER_DX12)
		backend = MARS_RENDERER_BACKEND_DX12;
		#else
		backend = MARS_RENDERER_BACKEND_VULKAN;
		#endif
	}
	return backend;
}