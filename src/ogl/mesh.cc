#include "mesh.hh"

#include <algorithm>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>


namespace ogl
{

Mesh::
Mesh(
	std::string const& file
	)
{
	load_obj(file);
	initialise_transform();
}

Mesh::
Mesh(
	Type type
	)
{
	auto positions = std::vector<glm::vec3>();
	auto normals   = std::vector<glm::vec3>();
	auto uvs       = std::vector<glm::vec2>();

	// Calculate data for type
	if (type == Triangle)
	{
		positions = {
			glm::vec3(0.75f, 0.0F, -0.5f),
			glm::vec3(0.0F, 0.0F, 1.0F),
			glm::vec3(-0.75f, 0.0F, -0.5f)
		};

		normals = {
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F)
		};

		uvs = {
			glm::vec2(1.0F, 0.0F),
			glm::vec2(0.5f, 1.0F),
			glm::vec2(0.0F, 0.0F)
		};

		size_ = 3;
	}
	else if (type == Quad)
	{
		positions = {
			glm::vec3(-0.5f, 0.0F, -0.5f),
			glm::vec3(0.5f, 0.0F, -0.5f),
			glm::vec3(-0.5f, 0.0F, 0.5f),
			glm::vec3(0.5f, 0.0F, -0.5f),
			glm::vec3(-0.5f, 0.0F, 0.5f),
			glm::vec3(0.5f, 0.0F, 0.5f)
		};

		normals = {
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F)
		};

		uvs = {
			glm::vec2(0.0F, 0.0F),
			glm::vec2(1.0F, 0.0F),
			glm::vec2(0.0F, 1.0F),
			glm::vec2(1.0F, 0.0F),
			glm::vec2(0.0F, 1.0F),
			glm::vec2(1.0F, 1.0F)
		};

		size_ = 6;
	}
	else if (type == Cube)
	{
		positions = {
			glm::vec3(-1.0F, -1.0F, -1.0F),
			glm::vec3(1.0F, -1.0F, -1.0F),
			glm::vec3(1.0F, 1.0F, -1.0F),
			glm::vec3(-1.0F, -1.0F, -1.0F),
			glm::vec3(1.0F, 1.0F, -1.0F),
			glm::vec3(-1.0F, 1.0F, -1.0F),
			glm::vec3(-1.0F, -1.0F, 1.0F),
			glm::vec3(1.0F, -1.0F, 1.0F),
			glm::vec3(1.0F, 1.0F, 1.0F),
			glm::vec3(-1.0F, -1.0F, 1.0F),
			glm::vec3(1.0F, 1.0F, 1.0F),
			glm::vec3(-1.0F, 1.0F, 1.0F),
			glm::vec3(-1.0F, -1.0F, -1.0F),
			glm::vec3(1.0F, -1.0F, -1.0F),
			glm::vec3(1.0F, -1.0F, 1.0F),
			glm::vec3(-1.0F, -1.0F, -1.0F),
			glm::vec3(1.0F, -1.0F, 1.0F),
			glm::vec3(-1.0F, -1.0F, 1.0F),
			glm::vec3(-1.0F, 1.0F, -1.0F),
			glm::vec3(1.0F, 1.0F, -1.0F),
			glm::vec3(1.0F, 1.0F, 1.0F),
			glm::vec3(-1.0F, 1.0F, -1.0F),
			glm::vec3(1.0F, 1.0F, 1.0F),
			glm::vec3(-1.0F, 1.0F, 1.0F),
			glm::vec3(-1.0F, -1.0F, -1.0F),
			glm::vec3(-1.0F, 1.0F, -1.0F),
			glm::vec3(-1.0F, 1.0F, 1.0F),
			glm::vec3(-1.0F, -1.0F, -1.0F),
			glm::vec3(-1.0F, 1.0F, 1.0F),
			glm::vec3(-1.0F, -1.0F, 1.0F),
			glm::vec3(1.0F, -1.0F, -1.0F),
			glm::vec3(1.0F, 1.0F, -1.0F),
			glm::vec3(1.0F, 1.0F, 1.0F),
			glm::vec3(1.0F, -1.0F, -1.0F),
			glm::vec3(1.0F, 1.0F, 1.0F),
			glm::vec3(1.0F, -1.0F, 1.0F)
		};

		normals = {
			glm::vec3(0.0F, 0.0F, -1.0F),
			glm::vec3(0.0F, 0.0F, -1.0F),
			glm::vec3(0.0F, 0.0F, -1.0F),
			glm::vec3(0.0F, 0.0F, -1.0F),
			glm::vec3(0.0F, 0.0F, -1.0F),
			glm::vec3(0.0F, 0.0F, -1.0F),
			glm::vec3(0.0F, 0.0F, 1.0F),
			glm::vec3(0.0F, 0.0F, 1.0F),
			glm::vec3(0.0F, 0.0F, 1.0F),
			glm::vec3(0.0F, 0.0F, 1.0F),
			glm::vec3(0.0F, 0.0F, 1.0F),
			glm::vec3(0.0F, 0.0F, 1.0F),
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, -1.0F, 0.0F),
			glm::vec3(0.0F, 1.0F, 0.0F),
			glm::vec3(0.0F, 1.0F, 0.0F),
			glm::vec3(0.0F, 1.0F, 0.0F),
			glm::vec3(0.0F, 1.0F, 0.0F),
			glm::vec3(0.0F, 1.0F, 0.0F),
			glm::vec3(0.0F, 1.0F, 0.0F),
			glm::vec3(-1.0F, 0.0F, 0.0F),
			glm::vec3(-1.0F, 0.0F, 0.0F),
			glm::vec3(-1.0F, 0.0F, 0.0F),
			glm::vec3(-1.0F, 0.0F, 0.0F),
			glm::vec3(-1.0F, 0.0F, 0.0F),
			glm::vec3(-1.0F, 0.0F, 0.0F),
			glm::vec3(1.0F, 0.0F, 0.0F),
			glm::vec3(1.0F, 0.0F, 0.0F),
			glm::vec3(1.0F, 0.0F, 0.0F),
			glm::vec3(1.0F, 0.0F, 0.0F),
			glm::vec3(1.0F, 0.0F, 0.0F),
			glm::vec3(1.0F, 0.0F, 0.0F)
		};

		size_ = 36;
	}

	obj::Mesh mesh;
	for (auto i = 0U; i < size_; ++i)
		mesh.vertices.push_back({ positions[i], normals[i], uvs[i] });

	// Create mesh
	initialise_mesh(mesh);
	initialise_transform();
}

Mesh::
~Mesh(
	)
{
	glDeleteVertexArrays(1, &vao_);

	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &nbo_);
	glDeleteBuffers(1, &ubo_);
}



// Attributes
auto Mesh::
size(
	) const
	-> GLuint
{
	return size_;
}

auto Mesh::
dimensions(
	) const
	-> glm::vec3
{
	return dimensions_;
}

auto Mesh::
vertices(
	) const
	-> std::vector<obj::Vertex>
{
	return vertices_;
}



// Buffers
auto Mesh::
vao(
	) const
	-> GLuint
{
	return vao_;
}

auto Mesh::
vbo(
	) const
	-> GLuint
{
	return vbo_;
}

auto Mesh::
nbo(
	) const
	-> GLuint
{
	return nbo_;
}

auto Mesh::
ubo(
	) const
	-> GLuint
{
	return ubo_;
}



// Matrices
auto Mesh::
reset_transforms(
	)
	-> void
{
	initialise_transform();
}

auto Mesh::
translate(
	) const
	-> glm::mat4
{
	return translate_;
}

auto Mesh::
translate(
	glm::vec3 vector
	)
	-> void
{
	translate_ = glm::translate(translate_, vector);
}

auto Mesh::
rotate(
	) const
	-> glm::mat4
{
	return rotate_;
}

auto Mesh::
rotate(
	float     angle,
	glm::vec3 vector
	)
	-> void
{
	rotate_ = glm::rotate(rotate_, angle, vector);
}

auto Mesh::
scale(
	) const
	-> glm::mat4
{
	return scale_;
}

auto Mesh::
scale(
	glm::vec3 vector
	)
	-> void
{
	scale_ = glm::scale(scale_, vector);
}



// Position
auto Mesh::
position(
	glm::vec3 position
	)
	-> void
{
	translate_[3] = glm::vec4(position, 1.0F);
}



// Load from file
auto Mesh::
load_obj(
	std::string const& file
	)
	-> bool
{
	// Load file
	if (!obj_.load(file))
		return false;

	initialise_mesh(obj_.get_meshes().front());
	return true;
}



// Initialise
auto Mesh::
initialise_transform(
	)
	-> void
{
	translate_ = glm::mat4(1.0F);
	rotate_    = glm::mat4(1.0F);
	scale_     = glm::mat4(1.0F);
}

auto Mesh::
initialise_mesh(
	obj::Mesh const& mesh
	)
	-> void
{
	// Store unique vertices
	vertices_ = mesh.vertices;
	vertices_.erase(
		std::unique(
			vertices_.begin(),
			vertices_.end()),
		vertices_.end());

	// Get positions and normals
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	for (auto const& v : mesh.vertices)
	{
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



// Vertex operations
auto Mesh::
calculate_dimensions(
	std::vector<glm::vec3> const& vertices
	)
	-> glm::vec3
{
	auto max = vertices.front();
	auto min = vertices.front();

	for (auto const& vertex : vertices)
		for (auto i = 0; i < 3; ++i)
			if (vertex[i] < min[i])
				min[i] = vertex[i];
			else if (vertex[i] > max[i])
				max[i] = vertex[i];

	return max - min;
}

} // namespace ogl
