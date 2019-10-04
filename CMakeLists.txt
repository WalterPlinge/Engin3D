# Settings
cmake_minimum_required(VERSION 3.11)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
project(Engin3D)

#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
#set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)

## Ouput all DLLs from all libs into main build folder
#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

## OpenGL
find_package(OpenGL REQUIRED)

# Libraries
set(glew-cmake_BUILD_SHARED        OFF CACHE BOOL "" FORCE)
set(glew-cmake_BUILD_MULTI_CONTEXT OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glew-cmake)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLM_TEST_ENABLE   OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glm)

## Engin3D
add_subdirectory(src)
