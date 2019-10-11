#pragma once

#include <string>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../obj/obj.hh"
#include "shader.hh"

namespace ogl
{

class Mesh
{
	// Variables
	GLuint                   size_       = 0;
	glm::vec3                dimensions_ = glm::vec3(1.0F);
	std::vector<obj::Vertex> vertices_;
	obj::Obj                 obj_;

	// Buffers
	GLuint vao_ = 0;
	GLuint vbo_ = 0;
	GLuint nbo_ = 0;
	GLuint ubo_ = 0;

	// Matrices
	glm::mat4 translate_ = glm::mat4(1.0F);
	glm::mat4 rotate_    = glm::mat4(1.0F);
	glm::mat4 scale_     = glm::mat4(1.0F);

public:

	// Shader
	Shader shader;



	// Base types
	enum Type {
		Triangle,
		Quad,
		Cube,
	};



	// Constructors
	explicit
	Mesh(
		std::string const& file
		);

	explicit
	Mesh(
		Type type = Triangle
		);

	~Mesh(
		);



	// Attributes
	auto
	size(
		) const
		-> GLuint;

	auto
	dimensions(
		) const
		-> glm::vec3;

	auto
	vertices(
		) const
		-> std::vector<obj::Vertex>;



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
	reset_transforms(
		)
		-> void;

	auto
	translate(
		) const
		-> glm::mat4;

	auto
	translate(
		glm::vec3 vector
		)
		-> void;

	auto
	rotate(
		) const
		-> glm::mat4;

	auto
	rotate(
		float     angle,
		glm::vec3 vector
		)
		-> void;

	auto
	scale(
		) const
		-> glm::mat4;

	auto
	scale(
		glm::vec3 vector
		)
		-> void;



	// Position
	auto
	position(
		glm::vec3 p
		)
		-> void;



	// Load from file
	auto
	load_obj(
		std::string const& file
		)
		-> bool;

private:

	// Initialise
	auto
	initialise_transform(
		)
		-> void;

	auto
	initialise_mesh(
		obj::Mesh const& mesh
		)
		-> void;



	// Vertex operations
	auto static
	calculate_dimensions(
		std::vector<glm::vec3> const& vertices
		)
		-> glm::vec3;
};

} // namespace ogl
