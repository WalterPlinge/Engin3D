### Settings

## Version
cmake_minimum_required(VERSION 3.15)

## Disable in-source builds
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

## Project
project(Engin3D
	LANGUAGES CXX
)

# These are for static builds
# Set ON to build this library like this
# Copy them to main cmake file if you want them for your whole project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR ENGIN3D_BUILD_SETTINGS)

	# Build
	set(CMAKE_CXX_EXTENSIONS OFF)
	set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
	set(BUILD_TESTING                OFF CACHE BOOL "" FORCE)
	option(BUILD_SHARED_LIBS      "" OFF)

	# Organisation
	set(GLOBAL PROPERTY USE_FOLDERS ON)

	# GLEW
	set(glew-cmake_BUILD_SHARED        OFF CACHE BOOL "" FORCE)
	set(glew-cmake_BUILD_MULTI_CONTEXT OFF CACHE BOOL "" FORCE)

	# GLFW
	set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

	# GLM
	set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
	set(GLM_TEST_ENABLE   OFF CACHE BOOL "" FORCE)

endif()



### Resources
add_custom_target(copy_resources ALL
	COMMAND ${CMAKE_COMMAND} -E
	copy_directory
		${CMAKE_SOURCE_DIR}/resources
		${CMAKE_BINARY_DIR}/bin/resources
)



### Libraries

# OpenGL
find_package(OpenGL REQUIRED)

# GLEW
add_subdirectory(lib/glew-cmake)

# GLFW
add_subdirectory(lib/glfw)

# GLM
add_subdirectory(lib/glm)



# Engin3D
add_subdirectory(src)



if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR ENGIN3D_BUILD_EXAMPLE)
	add_subdirectory(example)
endif()



if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR ENGIN3D_BUILD_SETTINGS)

	set_target_properties(Engin3D libglew_static glfw
		PROPERTIES
			MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>
			VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${CONFIG}
	)

endif()
