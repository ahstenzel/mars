#ifndef MARS_H
#define MARS_H
/**
 * mars.h
 * Single include to integrate Mars functionality.
*/

// Standard utilities
#include "mars/std/platform.h"
#include "mars/std/utilities.h"
#include "mars/std/debug.h"
#ifndef MARS_EXCLUDE_CONTAINERS
#include "mars/std/deque.h"
#include "mars/std/free_list.h"
#include "mars/std/priority_queue.h"
#include "mars/std/queue.h"
#include "mars/std/stack.h"
#include "mars/std/unordered_map.h"
#include "mars/std/unordered_map_str.h"
#include "mars/std/vector.h"
#endif

// Library functionality
#include "mars/game.h"

#endif // MARS_H