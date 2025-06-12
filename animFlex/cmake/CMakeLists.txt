cmake_minimum_required(VERSION 3.13)

# Project name.
project(animFlex)

# Various dependency directories variables.
set(SRC_DIR ../source)
set(GLAD_DIR ../../../glad/include)
set(GLM_DIR ../../../glm/glm)
set(STB_DIR ../../../stb)
set(IMGUI_DIR ../../../imgui/imgui)
set(IMGUI_BCK_DIR ../../../imgui/imgui/backends)
set(TRUETYPE_DIR ../../../freetype/freetype-2.10.0/include)
set(ZSTD_DIR ../../../zstd/lib)

# ZSTD library.
set(ZSTD_LIB_PATH "${CMAKE_SOURCE_DIR}/../libraries/libzstd.a")

# Collect source files.
file(GLOB_RECURSE SOURCES 
	${SRC_DIR}/*.cpp								# All sources.	
	${SRC_DIR}/third_party/*.c						# Third party c.
	${SRC_DIR}/third_party/*.cpp					# Third party cpp.
	${SRC_DIR}/third_party/*.cc						# Third party cc.
	${SRC_DIR}/third_party/*.hpp					# Third party hpp.
)

# Include directories for headers.
include_directories(
	${GLAD_DIR}										# GLAD header.
	${GLM_DIR}										# GLM header.
	${STB_DIR}										# STB header.
	${IMGUI_DIR}									# IMGUI header.
	${IMGUI_BCK_DIR}								# IMGUI backends header.
	${TRUETYPE_DIR}									# TRUETYPE headers.
	${ZSTD_DIR}										# ZSTD headers.
)

# Specify output file and target properties.
set(CMAKE_EXECUTABLE_SUFFIX ".html")

# Path to custom HTML shell file.
get_filename_component(PARENT_DIR "${CMAKE_SOURCE_DIR}" DIRECTORY)
set(CUSTOM_SHELL_FILE "${PARENT_DIR}/webassembly/template.html")

add_executable(animFlex ${SOURCES})

# Link zstd.
target_link_libraries(animFlex ${ZSTD_LIB_PATH})

# Apply flags to targets.
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
-s WASM=1 \
-s USE_GLFW=3 \
-s FULL_ES3=1 \
-s INITIAL_MEMORY=64MB \
-s MAXIMUM_MEMORY=512MB \
-s ALLOW_MEMORY_GROWTH=1 \
-s FORCE_FILESYSTEM \
--preload-file ../../content \
--preload-file ../../config \
--shell-file=${CUSTOM_SHELL_FILE} \
-s EXPORTED_RUNTIME_METHODS=['ccall'] \
-s USE_FREETYPE=1"
)