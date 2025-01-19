cmake_minimum_required(VERSION 3.13)

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

# Path custom HTML shell file.
get_filename_component(PARENT_DIR "${CMAKE_SOURCE_DIR}" DIRECTORY)
set(CUSTOM_SHELL_FILE "${PARENT_DIR}/webassembly/template.html")

add_executable(animFlex ${SOURCES})

# Set additional linker flags directly on the target.
set_target_properties(animFlex PROPERTIES LINK_FLAGS 
"-s WASM=1 -s USE_GLFW=3 -s FULL_ES3=1 -s FORCE_FILESYSTEM \
--shell-file ${CUSTOM_SHELL_FILE} \
-s EXPORTED_RUNTIME_METHODS=['ccall']")