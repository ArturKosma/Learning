cmake_minimum_required(VERSION 3.0)

# Project name.
project(animFlex)

# Various dependency directories variables.
set(SRC_DIR ../source)
set(GLAD_DIR ../../../glad/include)

# Collect source files.
file(GLOB_RECURSE SOURCES 
	${SRC_DIR}/*.cpp								# All sources.	
	${SRC_DIR}/third_party/glad.c					# GLAD loader.
)

# Include directories for headers.
include_directories(
	${GLAD_DIR}										# GLAD header.
)

# Set Emscripten-specific flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s USE_GLFW=3 -s FULL_ES3=1 -s FORCE_FILESYSTEM")

# Specify output file and target properties.
set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(animFlex ${SOURCES})

# Set additional linker flags directly on the target.
set_target_properties(animFlex PROPERTIES LINK_FLAGS "-s WASM=1 -s USE_GLFW=3 -s FULL_ES3=1 -s FORCE_FILESYSTEM")