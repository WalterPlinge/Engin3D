#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "../obj/obj.hh"
#include "shader.hh"

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
	std::uintmax_t  size_     = 0;

	// Object representation
	obj::Obj        obj_;

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
		Type               type = Empty,
		std::string const& file = std::string()
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
	reset_transforms(
		)
		-> void;

	auto
	euler_rotate(
		glm::vec3 angles
		)
		-> void;

	auto
	rotate(
		glm::quat q
		)
		-> void;



	// Matrices
	auto
	translation(
		) const
		-> glm::mat4;

	auto
	rotation(
		) const
		-> glm::mat4;

	auto
	scalar(
		) const
		-> glm::mat4;



	// Load model or file
	auto
	load(
		Type               type = Empty,
		std::string const& file = std::string()
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
