add_executable(${PROJECT_NAME}
##	main.hh
	main.cc

# Objects
	obj/obj.hh
	obj/obj.cc

# OpenGL
	ogl/app.hh
	ogl/app.cc
	ogl/camera.hh
	ogl/camera.cc
	ogl/mesh.hh
	ogl/mesh.cc
	ogl/shader.hh
	ogl/shader.cc
)
target_include_directories(${PROJECT_NAME}
	PUBLIC
		src
		lib
)
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		libglew_static
		glfw
		glm
)
target_compile_features(${PROJECT_NAME}
	PUBLIC
		cxx_std_17
)
set(source "${CMAKE_SOURCE_DIR}/shaders")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/shaders")
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
    DEPENDS ${destination}
    COMMENT "symbolic link resources folder from ${source} => ${destination}"
)
