#include <e3d/ogl/mesh.hh>

#include <algorithm>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>


namespace ogl
{

// Vertex operations
auto static
calculate_dimensions(
	std::vector<obj::Vertex> const& vertices
	)
	-> glm::vec3
{
	auto max = vertices.front().position;
	auto min = vertices.front().position;

	for (auto const& vertex : vertices)
		for (auto i = 0; i < 3; ++i)
			if (vertex.position[i] < min[i])
				min[i] = vertex.position[i];
			else if (vertex.position[i] > max[i])
				max[i] = vertex.position[i];

	return max - min;
}



Mesh::
Mesh(
	Type        const  type,
	std::string const& file
	)
{
	initialise_transform();
	load(type, file);
}

Mesh::
~Mesh(
	)
{
	clean();
}



// Attributes
auto Mesh::
size(
	) const
	-> std::uintmax_t
{
	return size_;
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
	glm::vec3 const vector
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
	float     const angle,
	glm::vec3 const vector
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
	glm::vec3 const vector
	)
	-> void
{
	scale_ = glm::scale(scale_, vector);
}



// Position
auto Mesh::
position(
	glm::vec3 const position
	)
	-> void
{
	translate_[3] = glm::vec4(position, 1.0F);
}



// Load model or file
auto Mesh::
load(
	Type        const  type,
	std::string const& file
	)
	-> bool
{
	if (type == Empty)
	{
		clean();
	}
	else if (type == File)
	{
		// Load file
		if (!obj_.load(file))
		{
			std::cerr << "ERROR: Could not load " <<
				file << std::endl;
			return false;
		}

		initialise_mesh(obj_.get_meshes().front());
	}
	else
	{
		auto positions = std::vector<glm::vec3>();
		auto normals   = std::vector<glm::vec3>();

		// Calculate data for type
		if (type == Triangle)
		{
			positions = {
				glm::vec3( 0.0F, 0.0F,  1.0F),
				glm::vec3(-1.0F, 0.0F, -1.0F),
				glm::vec3( 1.0F, 0.0F, -1.0F)
			};

			normals = {
				glm::vec3(0.0F, -1.0F, 0.0F),
				glm::vec3(0.0F, -1.0F, 0.0F),
				glm::vec3(0.0F, -1.0F, 0.0F)
			};

			size_ = 3;
		}
		else if (type == Quad)
		{
			positions = {
				glm::vec3( 1.0F,  1.0F, 0.0F),
				glm::vec3(-1.0F,  1.0F, 0.0F),
				glm::vec3(-1.0F, -1.0F, 0.0F),
				glm::vec3(-1.0F, -1.0F, 0.0F),
				glm::vec3( 1.0F, -1.0F, 0.0F),
				glm::vec3( 1.0F,  1.0F, 0.0F),
			};

			normals = {
				glm::vec3(0.0F, 0.0F, 1.0F),
				glm::vec3(0.0F, 0.0F, 1.0F),
				glm::vec3(0.0F, 0.0F, 1.0F),
				glm::vec3(0.0F, 0.0F, 1.0F),
				glm::vec3(0.0F, 0.0F, 1.0F),
				glm::vec3(0.0F, 0.0F, 1.0F)
			};

			size_ = 6;
		}
		else if (type == Cube)
		{
			positions = {
				// RIGHT
				glm::vec3(1.0F,  1.0F,  1.0F),
				glm::vec3(1.0F, -1.0F,  1.0F),
				glm::vec3(1.0F, -1.0F, -1.0F),
				glm::vec3(1.0F, -1.0F, -1.0F),
				glm::vec3(1.0F,  1.0F, -1.0F),
				glm::vec3(1.0F,  1.0F,  1.0F),

				// BACK
				glm::vec3( 1.0F, 1.0F,  1.0F),
				glm::vec3( 1.0F, 1.0F, -1.0F),
				glm::vec3(-1.0F, 1.0F, -1.0F),
				glm::vec3(-1.0F, 1.0F, -1.0F),
				glm::vec3(-1.0F, 1.0F,  1.0F),
				glm::vec3( 1.0F, 1.0F,  1.0F),

				// TOP
				glm::vec3( 1.0F,  1.0F, 1.0F),
				glm::vec3(-1.0F,  1.0F, 1.0F),
				glm::vec3(-1.0F, -1.0F, 1.0F),
				glm::vec3(-1.0F, -1.0F, 1.0F),
				glm::vec3( 1.0F, -1.0F, 1.0F),
				glm::vec3( 1.0F,  1.0F, 1.0F),

				// LEFT
				glm::vec3(-1.0F,  1.0F,  1.0F),
				glm::vec3(-1.0F,  1.0F, -1.0F),
				glm::vec3(-1.0F, -1.0F, -1.0F),
				glm::vec3(-1.0F, -1.0F, -1.0F),
				glm::vec3(-1.0F, -1.0F,  1.0F),
				glm::vec3(-1.0F,  1.0F,  1.0F),

				// FRONT
				glm::vec3( 1.0F, -1.0F,  1.0F),
				glm::vec3(-1.0F, -1.0F,  1.0F),
				glm::vec3(-1.0F, -1.0F, -1.0F),
				glm::vec3(-1.0F, -1.0F, -1.0F),
				glm::vec3( 1.0F, -1.0F, -1.0F),
				glm::vec3( 1.0F, -1.0F,  1.0F),

				// BOTTOM
				glm::vec3( 1.0F,  1.0F, -1.0F),
				glm::vec3( 1.0F, -1.0F, -1.0F),
				glm::vec3(-1.0F, -1.0F, -1.0F),
				glm::vec3(-1.0F, -1.0F, -1.0F),
				glm::vec3(-1.0F,  1.0F, -1.0F),
				glm::vec3( 1.0F,  1.0F, -1.0F)
			};

			normals = {
				// RIGHT
				glm::vec3(1.0F, 0.0F, 0.0F),
				glm::vec3(1.0F, 0.0F, 0.0F),
				glm::vec3(1.0F, 0.0F, 0.0F),
				glm::vec3(1.0F, 0.0F, 0.0F),
				glm::vec3(1.0F, 0.0F, 0.0F),
				glm::vec3(1.0F, 0.0F, 0.0F),

				// BACK
				glm::vec3(0.0F, 1.0F, 0.0F),
				glm::vec3(0.0F, 1.0F, 0.0F),
				glm::vec3(0.0F, 1.0F, 0.0F),
				glm::vec3(0.0F, 1.0F, 0.0F),
				glm::vec3(0.0F, 1.0F, 0.0F),
				glm::vec3(0.0F, 1.0F, 0.0F),

				// TOP
				glm::vec3(0.0F, 0.0F, 1.0F),
				glm::vec3(0.0F, 0.0F, 1.0F),
				glm::vec3(0.0F, 0.0F, 1.0F),
				glm::vec3(0.0F, 0.0F, 1.0F),
				glm::vec3(0.0F, 0.0F, 1.0F),
				glm::vec3(0.0F, 0.0F, 1.0F),

				// LEFT
				glm::vec3(-1.0F, 0.0F, 0.0F),
				glm::vec3(-1.0F, 0.0F, 0.0F),
				glm::vec3(-1.0F, 0.0F, 0.0F),
				glm::vec3(-1.0F, 0.0F, 0.0F),
				glm::vec3(-1.0F, 0.0F, 0.0F),
				glm::vec3(-1.0F, 0.0F, 0.0F),

				// FRONT
				glm::vec3(0.0F, -1.0F, 0.0F),
				glm::vec3(0.0F, -1.0F, 0.0F),
				glm::vec3(0.0F, -1.0F, 0.0F),
				glm::vec3(0.0F, -1.0F, 0.0F),
				glm::vec3(0.0F, -1.0F, 0.0F),
				glm::vec3(0.0F, -1.0F, 0.0F),

				// BOTTOM
				glm::vec3(0.0F, 0.0F, -1.0F),
				glm::vec3(0.0F, 0.0F, -1.0F),
				glm::vec3(0.0F, 0.0F, -1.0F),
				glm::vec3(0.0F, 0.0F, -1.0F),
				glm::vec3(0.0F, 0.0F, -1.0F),
				glm::vec3(0.0F, 0.0F, -1.0F)

			};

			size_ = 36;
		}

		auto mesh = obj::Mesh();
		for (auto i = 0U; i < size_; ++i)
		{
			auto v = obj::Vertex();
			v.position = positions[i];
			v.normal   = normals[i];
			mesh.vertices.push_back(v);
		}

		// Create mesh
		initialise_mesh(mesh);
	}

	return true;
}



// Clean up mesh
auto Mesh::
clean(
	)
	-> void
{
	if (vao_)
		glDeleteVertexArrays(1, &vao_);
	if (vbo_)
		glDeleteBuffers(1, &vbo_);
	if (nbo_)
		glDeleteBuffers(1, &nbo_);

	type_ = Empty;
	size_ = 0;
	obj_.clear();
	vao_ = 0;
	vbo_ = 0;
	nbo_ = 0;
	initialise_transform();
	shader.reset();
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
	size_ = mesh.vertices.size();

	// Get positions and normals
	auto positions = std::vector<glm::vec3>();
	auto normals = std::vector<glm::vec3>();

	positions.reserve(size_);
	normals.reserve(size_);

	for (auto const& v : mesh.vertices)
	{
		positions.push_back(v.position);
		normals.push_back(v.normal);
	}



	// Create vertex array object
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);



	// Create vertex buffer, bind to location 0
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(
		GL_ARRAY_BUFFER,
		size_ * sizeof(positions.front()),
		positions.data(),
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);



	// Create normal buffer, bind to location 1
	glGenBuffers(1, &nbo_);
	glBindBuffer(GL_ARRAY_BUFFER, nbo_);
	glBufferData(
		GL_ARRAY_BUFFER,
		size_ * sizeof(normals.front()),
		normals.data(),
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);



	glBindVertexArray(0);
}

} // namespace ogl
