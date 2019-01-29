#include "mesh.hh"

///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <algorithm>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLM
#include <glm/gtc/matrix_transform.hpp>

///////////////////////////////////////////////////////////////
// RENDER NAMESPACE

namespace ogl {

	///////////////////////////////////////////////////////////
	// CONSTRUCTORS

	Mesh::Mesh(const std::string& file)
	{
		load_obj(file);

		initialise_transform();
	}

	Mesh::Mesh(const Type& type)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		// Calculate data for type
		if (type == Triangle) {
			// Vertices
			positions = {
				glm::vec3(0.75f, 0.0f, -0.5f),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(-0.75f, 0.0f, -0.5f)
			};

			// Normals
			normals = {
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f)
			};

			// Texture coordinates
			uvs = {
				glm::vec2(1.0f, 0.0f),
				glm::vec2(0.5f, 1.0f),
				glm::vec2(0.0f, 0.0f)
			};

			// Number of indices
			size_ = 3;
		} else if (type == Quad) {
			// Vertices
			positions = {
				glm::vec3(-0.5f, 0.0f, -0.5f),
				glm::vec3(0.5f, 0.0f, -0.5f),
				glm::vec3(-0.5f, 0.0f, 0.5f),
				glm::vec3(0.5f, 0.0f, -0.5f),
				glm::vec3(-0.5f, 0.0f, 0.5f),
				glm::vec3(0.5f, 0.0f, 0.5f)
			};

			// Normals
			normals = {
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f)
			};

			// Texture coordinates
			uvs = {
				glm::vec2(0.0f, 0.0f),
				glm::vec2(1.0f, 0.0f),
				glm::vec2(0.0f, 1.0f),
				glm::vec2(1.0f, 0.0f),
				glm::vec2(0.0f, 1.0f),
				glm::vec2(1.0f, 1.0f)
			};

			// Number of indices
			size_ = 6;
		}/* else if (type == Cube) {
			// Vertices
			positions = {
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, 1.0f, -1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, 1.0f, -1.0f),
				glm::vec3(-1.0f, 1.0f, -1.0f),
				glm::vec3(-1.0f, -1.0f, 1.0f),
				glm::vec3(1.0f, -1.0f, 1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(-1.0f, -1.0f, 1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(-1.0f, 1.0f, 1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, 1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, -1.0f, 1.0f),
				glm::vec3(-1.0f, -1.0f, 1.0f),
				glm::vec3(-1.0f, 1.0f, -1.0f),
				glm::vec3(1.0f, 1.0f, -1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(-1.0f, 1.0f, -1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(-1.0f, 1.0f, 1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, 1.0f, -1.0f),
				glm::vec3(-1.0f, 1.0f, 1.0f),
				glm::vec3(-1.0f, -1.0f, -1.0f),
				glm::vec3(-1.0f, 1.0f, 1.0f),
				glm::vec3(-1.0f, -1.0f, 1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, 1.0f, -1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, -1.0f, -1.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				glm::vec3(1.0f, -1.0f, 1.0f)
			};

			// Normals
			normals = {
				glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, -1.0f),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, 0.0f, 1.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, -1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f),
				glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(-1.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 0.0f, 0.0f)
			};

			// Number of indices
			size_ = 36;
		}*/

		obj::Mesh mesh;
		for (unsigned i(0); i < size_; ++i) {
			mesh.vertices.push_back({ positions[i], normals[i], uvs[i] });
		}

		// Create mesh
		initialise_mesh(mesh);

		initialise_transform();
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &vao_);

		glDeleteBuffers(1, &vbo_);
		glDeleteBuffers(1, &nbo_);
		glDeleteBuffers(1, &ubo_);
	}

	///////////////////////////////////////////////////////
	// GETTERS

	// Attributes
	GLuint Mesh::get_size() const
	{
		return size_;
	}

	glm::vec3 Mesh::get_dimensions() const
	{
		return dimensions_;
	}

	std::vector<obj::Vertex> Mesh::get_vertices() const
	{
		return vertices_;
	}

	// Buffers
	GLuint Mesh::get_vao() const
	{
		return vao_;
	}

	GLuint Mesh::get_vbo() const
	{
		return vbo_;
	}

	GLuint Mesh::get_nbo() const
	{
		return nbo_;
	}

	// Matrices
	glm::mat4 Mesh::get_translate() const
	{
		return translate_;
	}

	glm::mat4 Mesh::get_rotate() const
	{
		return rotate_;
	}

	glm::mat4 Mesh::get_scale() const
	{
		return scale_;
	}

	// Shader
	Shader Mesh::get_shader() const
	{
		return shader_;
	}

	///////////////////////////////////////////////////////
	// SETTERS

	void Mesh::set_position(const glm::vec3& position)
	{
		translate_[3] = glm::vec4(position, 1.0f);
	}

	///////////////////////////////////////////////////////
	// OBJ FILES

	bool Mesh::load_obj(const std::string& file)
	{
		// Load file
		if (!obj_.load(file))
			return false;

		initialise_mesh(obj_.get_meshes().front());

		return true;
	}

	///////////////////////////////////////////////////////
	// TRANSFORMS

	void Mesh::translate(const glm::vec3& vector)
	{
		translate_ = glm::translate(translate_, vector);
	}

	void Mesh::rotate(const float& angle, const glm::vec3& vector)
	{
		rotate_ = glm::rotate(rotate_, angle, vector);
	}

	void Mesh::scale(const glm::vec3& vector)
	{
		scale_ = glm::scale(scale_, vector);
	}

	void Mesh::reset_transforms()
	{
		initialise_transform();
	}

	///////////////////////////////////////////////////////
	// INITIALISE

	void Mesh::initialise_transform()
	{
		translate_ = glm::mat4(1.0f);
		rotate_ = glm::mat4(1.0f);
		scale_ = glm::mat4(1.0f);
	}

	void Mesh::initialise_mesh(const obj::Mesh& mesh)
	{
		// Store unique vertices
		vertices_ = mesh.vertices;
		vertices_.erase(std::unique(vertices_.begin(), vertices_.end()), vertices_.end());

		// Get positions and normals
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		for (const obj::Vertex& v : mesh.vertices) {
			positions.push_back(v.position);
			normals.push_back(v.normal);
			uvs.push_back(v.uv);
		}

		dimensions_ = calculate_dimensions(positions);

		// Create vertex array object
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		// Create vertex buffer, bind to location 0
		glGenBuffers(1, &vbo_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, size_ * sizeof positions.front(), positions.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// Create normal buffer, bind to location 1
		glGenBuffers(1, &nbo_);
		glBindBuffer(GL_ARRAY_BUFFER, nbo_);
		glBufferData(GL_ARRAY_BUFFER, size_ * sizeof normals.front(), normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// Create uv buffer, bind to location 2
		glGenBuffers(1, &ubo_);
		glBindBuffer(GL_ARRAY_BUFFER, ubo_);
		glBufferData(GL_ARRAY_BUFFER, size_ * sizeof uvs.front(), uvs.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindVertexArray(0);
	}

	///////////////////////////////////////////////////////
	// VERTEX OPERATIONS

	glm::vec3 Mesh::calculate_dimensions(const std::vector<glm::vec3>& vertices)
	{
		glm::vec3 max(vertices.front());
		glm::vec3 min(vertices.front());

		for (const glm::vec3& vertex : vertices)
			for (int i(0); i < 3; ++i)
				if (vertex[i] < min[i])
					min[i] = vertex[i];

				else if (vertex[i] > max[i])
					max[i] = vertex[i];

		return max - min;
	}
}
