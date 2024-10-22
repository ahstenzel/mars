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
	int _rendererBackend;
} DisplaySettingsList;

/// @brief Top level game rendering structure.
typedef struct {
	GLFWwindow* _window;
	void* _renderer;
} Display;

/// @brief Descriptor for creating the renderer
typedef struct {
	const char* name;		// Display title
} DisplayDesc;

Display* _CreateDisplay(DisplayDesc _desc);

void _DestroyDisplay(Display* _window);

void _UpdateDisplay(Display* _window);

bool _DisplayShouldClose(Display* _window);

int _GetRendererBackend();

#endif // MARS_DISPLAY_H