# Example project: Simple
project(example_triangle)

# Add source files
file (GLOB game_src
	"${PROJECT_SOURCE_DIR}/src/main.c"
)

# Set output
set(OUTPUT_TREE "bin/examples/triangle/$<IF:$<CONFIG:Debug>,Debug,Release>")
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/${OUTPUT_TREE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

# Build binary
add_executable(example_triangle ${game_src})

# Add inclusions
target_include_directories(example_triangle PUBLIC "${CMAKE_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/src")

# Add MARS libraries
target_link_directories(example_triangle PUBLIC ${BIN_DIR})
target_link_libraries(example_triangle PUBLIC mars)

# Platform configuration
if (WIN32)
	if (MSVC) 
		set_target_properties(
			example_triangle PROPERTIES 
			LINK_FLAGS_DEBUG "/NODEFAULTLIB:MSVCRT /SUBSYSTEM:CONSOLE"
			LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
		)
		target_compile_definitions(example_triangle PRIVATE _CRT_SECURE_NO_WARNINGS)
	elseif(MINGW)
		set_target_properties(
			example_triangle PROPERTIES 
			LINK_FLAGS_DEBUG "-Wl,-subsystem,console"
			LINK_FLAGS_RELEASE "-Wl,-subsystem,windows"
		)
	elseif(MSYS)
		set_target_properties(
			example_triangle PROPERTIES 
			LINK_FLAGS_DEBUG "-Wl,/subsystem:console"
			LINK_FLAGS_RELEASE "-Wl,/subsystem:windows,/entry:mainCRTStartup"
		)
	endif()
endif()