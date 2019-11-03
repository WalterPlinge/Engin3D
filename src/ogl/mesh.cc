#include <e3d/ogl/mesh.hh>

#include <algorithm>
#include <tuple>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>


namespace ogl
{

// Vertex operations
auto static
calculate_dimensions(
	std::vector<glm::vec3> const& vertices
	)
	-> std::tuple<glm::vec3, glm::vec3>
{
	auto max = vertices.front();
	auto min = vertices.front();

	for (auto const& v : vertices)
		for (auto i = 0; i < 3; ++i)
			if (v[i] < min[i])
				min[i] = v[i];
			else if (v[i] > max[i])
				max[i] = v[i];

	return std::tie(max, min);
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

auto Mesh::
ubo(
	) const
	-> GLuint
{
	return ubo_;
}



// Transforms
auto Mesh::
calculate_bounds(
	)
	-> void
{
	minimum = obj_.vertices.front().position;
	maximum = obj_.vertices.front().position;

	for (auto const& v : obj_.vertices)
		for (auto i = 0; i < 3; ++i)
			if (v.position[i] < minimum[i])
				minimum[i] = v.position[i];
			else if (v.position[i] > maximum[i])
				maximum[i] = v.position[i];
}

auto Mesh::
reset_transforms(
	)
	-> void
{
	initialise_transform();
}

auto Mesh::
rotate(
	glm::vec3 const euler_angles
	)
	-> void
{
	rotate(glm::quat(euler_angles));
}

auto Mesh::
rotate(
	glm::quat const q
	)
	-> void
{
	orientation = glm::normalize(orientation * q);
}



// Matrices
auto Mesh::
translation(
	) const
	-> glm::mat4
{
	return glm::translate(glm::mat4(1.0F), position);
}

auto Mesh::
rotation(
	) const
	-> glm::mat4
{
	return glm::mat4_cast(orientation);
}

auto Mesh::
scalar(
	) const
	-> glm::mat4
{
	return glm::scale(glm::mat4(1.0F), scale);
}



// Load model or file
auto Mesh::
load(
	Type        const  type,
	std::string const& file
	)
	-> bool
{
	type_ = type;
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
		auto uvs       = std::vector<glm::vec2>();

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

			uvs = {
				glm::vec2(0.5F, 1.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(1.0F, 0.0F)
			};
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

			uvs = {
				glm::vec2(1.0F, 1.0F),
				glm::vec2(0.0F, 1.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(1.0F, 0.0F),
				glm::vec2(1.0F, 1.0F)
			};
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

			uvs = {
				// RIGHT
				glm::vec2(1.0F, 1.0F),
				glm::vec2(0.0F, 1.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(1.0F, 0.0F),
				glm::vec2(1.0F, 1.0F),

				// BACK
				glm::vec2(1.0F, 1.0F),
				glm::vec2(0.0F, 1.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(1.0F, 0.0F),
				glm::vec2(1.0F, 1.0F),

				// TOP
				glm::vec2(1.0F, 1.0F),
				glm::vec2(0.0F, 1.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(1.0F, 0.0F),
				glm::vec2(1.0F, 1.0F),

				// LEFT
				glm::vec2(1.0F, 1.0F),
				glm::vec2(0.0F, 1.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(1.0F, 0.0F),
				glm::vec2(1.0F, 1.0F),

				// FRONT
				glm::vec2(1.0F, 1.0F),
				glm::vec2(0.0F, 1.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(1.0F, 0.0F),
				glm::vec2(1.0F, 1.0F),

				// BOTTOM
				glm::vec2(1.0F, 1.0F),
				glm::vec2(0.0F, 1.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(0.0F, 0.0F),
				glm::vec2(1.0F, 0.0F),
				glm::vec2(1.0F, 1.0F),
			};
		}

		size_ = positions.size();
		for (auto i = 0U; i < size_; ++i)
		{
			auto v = obj::Vertex();
			v.position = positions[i];
			v.normal   = normals[i];
			v.uv       = uvs[i];
			obj_.vertices.push_back(v);
		}

		// Create mesh
		initialise_mesh(positions, normals, uvs);
	}

	return true;
}

auto Mesh::
initialise_mesh(
	std::vector<glm::vec3> const& vertices,
	std::vector<glm::vec3> const& normals,
	std::vector<glm::vec2> const& uvs
	)
	-> void
{
	if (vertices.size() != normals.size())
	{
		std::cerr << "ERROR: Vertex and Normal count is unequal" << std::endl;
		return;
	}

	// Size and type
	if (type_ == Empty)
		type_ = Other;
	size_ = vertices.size();

	if (size_ == 0)
	{
		std::cerr << "ERROR: No vertices supplied" << std::endl;
		return;
	}

	// Limits
	calculate_bounds();



	// Create vertex array object
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// Create vertex buffer, bind to location 0
	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(
		GL_ARRAY_BUFFER,
		size_ * sizeof(vertices.front()),
		vertices.data(),
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

	// Create an empty list if uvs were not passed
	auto empty_uvs = std::vector<glm::vec2>();
	if (uvs.empty())
		empty_uvs.resize(size_);

	// Create uv buffer, bind to location 2
	glGenBuffers(1, &ubo_);
	glBindBuffer(GL_ARRAY_BUFFER, ubo_);
	glBufferData(
		GL_ARRAY_BUFFER,
		size_ * sizeof(uvs.front()),
		uvs.empty() ? empty_uvs.data() : uvs.data(),
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindVertexArray(0);
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
	if (ubo_)
		glDeleteBuffers(1, &ubo_);

	type_ = Empty;
	size_ = 0;
	obj_.clear();
	vao_ = 0;
	vbo_ = 0;
	nbo_ = 0;
	ubo_ = 0;
	initialise_transform();
	shader.reset();
}



// Initialise
auto Mesh::
initialise_transform(
	)
	-> void
{
	scale       = glm::vec3(1.0F);
	position    = glm::vec3(0.0F);
	orientation = glm::quat();
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
	auto normals   = std::vector<glm::vec3>();
	auto uvs       = std::vector<glm::vec2>();

	positions.reserve(size_);
	normals.reserve(size_);
	uvs.reserve(size_);

	for (auto const& v : mesh.vertices)
	{
		positions.push_back(v.position);
		normals.push_back(v.normal);
		uvs.push_back(v.uv);
	}

	initialise_mesh(positions, normals, uvs);
}

} // namespace ogl
