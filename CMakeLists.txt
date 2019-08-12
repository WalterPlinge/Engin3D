## Project settings
project(Engin3D)

cmake_minimum_required(VERSION 3.11)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)

## C++ version
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

## OpenGL
find_package(OpenGL REQUIRED)

## Load libraries
# GLEW
add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew-cmake)

# GLFW
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

# GLM
include_directories(lib/glm)

###

## Engin3D
file(GLOB_RECURSE SOURCES src/*.cc src/*.hh)
add_executable(Engin3D ${SOURCES})
target_link_libraries(Engin3D PUBLIC glfw libglew_static glm)
