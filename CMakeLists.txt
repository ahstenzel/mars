cmake_minimum_required(VERSION 3.13)
project(mars)

# Definitions
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin/mars/${MARS_OS}/static/${MARS_GEN}/$<IF:$<CONFIG:Debug>,Debug,Release>")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib/${MARS_OS}/static/${MARS_GEN}/$<IF:$<CONFIG:Debug>,Debug,Release>")

# Add source files
file(GLOB mars_src 
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
	"${SRC_DIR}/mars/game.c"
)

# Set output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# Build binary
add_library(mars ${mars_src})

# Add dependencies
target_include_directories(mars PUBLIC ${SRC_DIR})
target_link_directories(mars PUBLIC ${LIB_DIR})
target_link_libraries(mars PUBLIC winmm)

# Enforce standards
target_compile_features(mars PUBLIC c_std_11)

# Build examples