#pragma once

#include <memory>
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
	GLsizei                  size_ = 0;
	obj::Obj                 obj_;

	// Buffers
	GLuint vao_ = 0;
	GLuint vbo_ = 0;
	GLuint nbo_ = 0;

	// Matrices
	glm::mat4 translate_ = glm::mat4(1.0F);
	glm::mat4 rotate_    = glm::mat4(1.0F);
	glm::mat4 scale_     = glm::mat4(1.0F);

public:

	// Shader
	std::shared_ptr<Shader> shader;



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
		-> GLsizei;



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
		glm::vec3 position
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
};

} // namespace ogl
