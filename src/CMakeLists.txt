# Include directories
set(OBJ_DIR ../include/e3d/obj)
set(OGL_DIR ../include/e3d/ogl)

# Header files
set(HEADER_FILES
	${OBJ_DIR}/obj.hh

	${OGL_DIR}/app.hh
	${OGL_DIR}/camera.hh
	${OGL_DIR}/framebuffer.hh
	${OGL_DIR}/mesh.hh
	${OGL_DIR}/renderer.hh
	${OGL_DIR}/shader.hh
	${OGL_DIR}/texture.hh
)

# Source files
set(SOURCE_FILES
	obj/obj.cc

	ogl/app.cc
	ogl/camera.cc
	ogl/framebuffer.cc
	ogl/mesh.cc
	ogl/renderer.cc
	ogl/shader.cc
	ogl/texture.cc
)

add_library(Engin3D
	${HEADER_FILES}
	${SOURCE_FILES}
)

target_include_directories(Engin3D
	PUBLIC
		${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(Engin3D
	PUBLIC
		glm
		libglew_static
		glfw
)

target_compile_features(Engin3D
	PUBLIC
		cxx_std_17
)

source_group(TREE
	${PROJECT_SOURCE_DIR}/include
	PREFIX "Header Files"
	FILES ${HEADER_FILES}
)
