### Settings

# Version
cmake_minimum_required(VERSION 3.15)

# Disable in-source builds
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Project
project(Engin3D
	LANGUAGES CXX
)

# Build
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING                OFF CACHE BOOL "" FORCE)
option(BUILD_SHARED_LIBS      "" OFF)

# Organisation
set(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
#set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)

## Ouput all DLLs from all libs into main build folder
#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})



### Libraries

# OpenGL
find_package(OpenGL REQUIRED)

# GLEW
set(glew-cmake_BUILD_SHARED        OFF CACHE BOOL "" FORCE)
set(glew-cmake_BUILD_MULTI_CONTEXT OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glew-cmake)

# GLFW
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

# GLM
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE   OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glm)

### Resources
add_custom_target(copy_resources ALL
	COMMAND ${CMAKE_COMMAND} -E
	copy_directory
		${CMAKE_SOURCE_DIR}/res
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
)

## Engin3D
add_subdirectory(src)

set_target_properties(
		${PROJECT_NAME}
		libglew_static
		glfw
	PROPERTIES
		MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>
		VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG}
)
