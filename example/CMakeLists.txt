add_executable(Engin3D_Example main.cc)

target_link_libraries(Engin3D_Example
	PUBLIC
		Engin3D
)

target_compile_features(Engin3D_Example
	PUBLIC
		cxx_std_17
)

set_target_properties(Engin3D_Example PROPERTIES
	CXX_EXTENSIONS OFF
	FOLDER         Engin3D_Example
)

add_dependencies(Engin3D_Example
	copy_resources
)

set_target_properties(Engin3D
	PROPERTIES
		CXX_EXTENSIONS OFF
		FOLDER         Engin3D
)
