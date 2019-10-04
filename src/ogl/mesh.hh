#ifndef OGL_MESH_H
#define OGL_MESH_H

///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <string>
#include <vector>

// GLM
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// Project
#include "../obj/obj.hh"
#include "shader.hh"

///////////////////////////////////////////////////////////////
// RENDER NAMESPACE

namespace ogl {

	///////////////////////////////////////////////////////////
	// MESH CLASS

	class Mesh {
	public:

		///////////////////////////////////////////////////////
		// DEFAULT MESH TYPES

		enum Type {
			Triangle,
			Quad,
			//Cube,
		};

		///////////////////////////////////////////////////////
		// CONSTRUCTORS

		explicit Mesh(const std::string& file);
		explicit Mesh(const Type& type = Triangle);

		~Mesh();

		///////////////////////////////////////////////////////
		// GETTERS

		// Attributes
		GLuint get_size() const;
		glm::vec3 get_dimensions() const;
		std::vector<obj::Vertex> get_vertices() const;

		// Buffers
		GLuint get_vao() const;
		GLuint get_vbo() const;
		GLuint get_nbo() const;

		// Matrices
		glm::mat4 get_translate() const;
		glm::mat4 get_rotate() const;
		glm::mat4 get_scale() const;

		// Shader
		Shader get_shader() const;

		///////////////////////////////////////////////////////
		// SETTERS

		void set_position(const glm::vec3& position);

		///////////////////////////////////////////////////////
		// OBJ FILES

		bool load_obj(const std::string& file);

		///////////////////////////////////////////////////////
		// TRANSFORMS

		void translate(const glm::vec3& vector);
		void rotate(const float& angle, const glm::vec3& vector);
		void scale(const glm::vec3& vector);
		void reset_transforms();

	private:

		///////////////////////////////////////////////////////
		// INITIALISE

		void initialise_transform();
		void initialise_mesh(const obj::Mesh& mesh);

		///////////////////////////////////////////////////////
		// VERTEX OPERATIONS

		static glm::vec3 calculate_dimensions(const std::vector<glm::vec3>& vertices);

		///////////////////////////////////////////////////////
		// ATTRIBUTES

		// Variables
		GLuint size_ = 0;
		glm::vec3 dimensions_ = glm::vec3(1.0f);
		std::vector<obj::Vertex> vertices_;
		obj::Obj obj_;

		// Buffers
		GLuint vao_ = 0;
		GLuint vbo_ = 0;
		GLuint nbo_ = 0;
		GLuint ubo_ = 0;

		// Matrices
		glm::mat4 translate_ = glm::mat4(1.0f);
		glm::mat4 rotate_ = glm::mat4(1.0f);
		glm::mat4 scale_ = glm::mat4(1.0f);

		// Shader
		Shader shader_;
	};
}

#endif
