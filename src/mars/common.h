#ifndef MARS_COMMON_H
#define MARS_COMMON_H
/**
 * common.h
 * Common defines & includes for the whole library.
 */

//----------------------------------------------------------------------------------
// Header inclusions
//----------------------------------------------------------------------------------

// Standard includes
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <memory.h>

// OS includes
#include "mars/std/platform.h"

// Mars includes
#include "mars/std/debug.h"
#include "mars/std/deque.h"
#include "mars/std/free_list.h"
#include "mars/std/priority_queue.h"
#include "mars/std/queue.h"
#include "mars/std/stack.h"
#include "mars/std/unordered_map.h"
#include "mars/std/unordered_map_str.h"
#include "mars/std/utilities.h"
#include "mars/std/vector.h"
#include "mars/std/base64.h"

// External includes
#define INI_USE_STACK 0
#define INI_ALLOW_REALLOC 1
#include "external/inih/ini.h"
#define GLFW_INCLUDE_VULKAN
#include "external/GLFW/glfw3.h"
#include <shaderc/shaderc.h>

#endif // MARS_COMMON_H