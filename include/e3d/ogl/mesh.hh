#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include <glm/glm.hpp>

#include "../obj/obj.hh"
#include "shader.hh"

using namespace std::string_view_literals;

namespace ogl
{

class Mesh
{
public:

	// Base types
	enum Type {
		Empty,
		Triangle,
		Quad,
		Cube,
		File,
		Other
	};

private:

	Type type_ = Empty;

	// Buffers
	GLuint vao_ = 0U;
	GLuint vbo_ = 0U;
	GLuint nbo_ = 0U;
	GLuint ubo_ = 0U;

	// Details
	std::uintmax_t size_ = 0;

	// Object representation
	obj::Obj obj_;

public:

	// Details
	glm::vec3 scale    = glm::vec3(1.0F);
	glm::vec3 position = glm::vec3(0.0F);
	glm::quat orientation;

	// Bounds
	glm::vec3 minimum;
	glm::vec3 maximum;

	// Shader
	std::shared_ptr<Shader> shader;



	// Constructors
	explicit
	Mesh(
		Type             type = Empty,
		std::string_view file = ""sv
		);

	~Mesh(
		);



	// Attributes
	auto
	size(
		) const
		-> std::uintmax_t;



	// Buffers
	auto
	vao(
		) const
		-> GLuint;

	auto
	vbo(
		) const
		-> GLuint;

	auto
	nbo(
		) const
		-> GLuint;

	auto
	ubo(
		) const
		-> GLuint;



	// Transforms
	auto
	calculate_bounds(
		)
		-> void;

	auto
	reset_transforms(
		)
		-> void;

	auto
	rotate(
		float     angle,
		glm::vec3 axis
		)
		-> void;

	auto
	rotate(
		glm::vec3 euler_angles
		)
		-> void;

	auto
	rotate(
		glm::quat q
		)
		-> void;



	// Matrices
	auto
	translate_matrix(
		) const
		-> glm::mat4;

	auto
	rotate_matrix(
		) const
		-> glm::mat4;

	auto
	scale_matrix(
		) const
		-> glm::mat4;

	auto
	model_matrix(
		) const
		-> glm::mat4;

	auto
	normal_matrix(
		) const
		-> glm::mat3;



	// Load model or file
	auto
	load(
		Type             type = Empty,
		std::string_view file = ""sv
		)
		-> bool;

	auto
	initialise_mesh(
		std::vector<glm::vec3> const& vertices,
		std::vector<glm::vec3> const& normals,
		std::vector<glm::vec2> const& uvs = std::vector<glm::vec2>()
		)
		-> void;



	// Clean up mesh
	auto
	clean(
		)
		-> void;

private:

	// Initialise
	auto
	initialise_obj_mesh(
		)
		-> void;
};

} // namespace ogl
