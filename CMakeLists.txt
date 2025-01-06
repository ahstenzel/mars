cmake_minimum_required(VERSION 3.13)
project(mars)

# Library type detection
if(MARS_BUILD_SHARED)
	set(BUILD_SHARED_LIBS true)
endif()

# Definitions
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(RES_DIR "${CMAKE_SOURCE_DIR}/res")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin/mars/${MARS_OS}/${MARS_LIB}/${MARS_GEN}/$<IF:$<CONFIG:Debug>,Debug,Release>")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib/${MARS_LIB}/${MARS_GEN}")

# Add source files
set(mars_src 
	"${SRC_DIR}/mars/std/deque.c"
	"${SRC_DIR}/mars/std/free_list.c"
	"${SRC_DIR}/mars/std/priority_queue.c"
	"${SRC_DIR}/mars/std/queue.c"
	"${SRC_DIR}/mars/std/stack.c"
	"${SRC_DIR}/mars/std/unordered_map.c"
	"${SRC_DIR}/mars/std/unordered_map_str.c"
	"${SRC_DIR}/mars/std/utilities.c"
	"${SRC_DIR}/mars/std/vector.c"
	"${SRC_DIR}/mars/std/debug.c"
	"${SRC_DIR}/mars/std/base64.c"
	"${SRC_DIR}/mars/std/buffer.c"
	"${SRC_DIR}/mars/game.c"
	"${SRC_DIR}/mars/input.c"
	"${SRC_DIR}/mars/display.c"
	"${SRC_DIR}/mars/settings.c"
	"${SRC_DIR}/mars/renderer_vk.c"
	"${SRC_DIR}/mars/resource.c"
	"${SRC_DIR}/external/inih/ini.c"
	"${SRC_DIR}/external/aes/aes.c"
	"${SRC_DIR}/external/lz4/lz4.c"
	"${SRC_DIR}/external/stb/stb_image.c"
)

# Set outputs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Find external libraries
find_package(Vulkan REQUIRED)

# Build binary
add_library(mars ${mars_src})
if(MARS_GEN STREQUAL "msvc2022")
    target_compile_definitions(mars PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()

# Add dependencies
target_include_directories(mars 
	PUBLIC ${SRC_DIR}
	PUBLIC ${SRC_DIR}/external/
	PUBLIC ${SRC_DIR}/external/inih
	PUBLIC ${SRC_DIR}/external/aes
	PUBLIC ${SRC_DIR}/external/lz4
	PUBLIC ${VULKAN_INCLUDE_DIRS}
)
target_link_directories(mars PUBLIC ${LIB_DIR})
target_link_libraries(mars PUBLIC winmm glfw3 Vulkan::Vulkan)

# Platform configuration
if (OS STREQUAL "windows")
	if (GEN STREQUAL "msvc2022") 
		set_target_properties(
			example_simple PROPERTIES 
			LINK_FLAGS_DEBUG "/NODEFAULTLIB:MSVCRT /SUBSYSTEM:CONSOLE"
			LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
		)
		target_compile_definitions(example_simple PRIVATE _CRT_SECURE_NO_WARNINGS)
	elseif(GEN STREQUAL "mingw")
		set_target_properties(
			example_simple PROPERTIES 
			LINK_FLAGS_DEBUG "-Wl,-subsystem,console"
			LINK_FLAGS_RELEASE "-Wl,-subsystem,windows"
		)
	elseif(GEN STREQUAL "clang")
		set_target_properties(
			example_simple PROPERTIES 
			LINK_FLAGS_DEBUG "-Wl,/subsystem:console"
			LINK_FLAGS_RELEASE "-Wl,/subsystem:windows,/entry:mainCRTStartup"
		)
	endif()
endif()

# Enforce standards
target_compile_features(mars PUBLIC c_std_11)

# Build examples
add_subdirectory(examples/triangle)