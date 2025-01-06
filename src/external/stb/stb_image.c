#include "mars/std/debug.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_MALLOC MARS_MALLOC
#define STB_REALLOC MARS_REALLOC
#define STB_FREE MARS_FREE
#define STBI_ASSERT(x) MARS_ASSERT(x)
#include "stb_image.h"