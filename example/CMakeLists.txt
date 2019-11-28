add_executable(Engin3D_Example main.cc)

target_link_libraries(Engin3D_Example
	PUBLIC
		Engin3D
)

target_compile_features(Engin3D_Example
	PUBLIC
		cxx_std_20
)

set_target_properties(Engin3D_Example
	PROPERTIES
		CXX_EXTENSIONS           OFF
		FOLDER                   Engin3D_Example
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_dependencies(Engin3D_Example
	copy_resources
)
