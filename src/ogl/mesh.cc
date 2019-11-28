#include <e3d/ogl/mesh.hh>

#include <functional>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/ext.hpp>

#include <e3d/ogl/geometry.hh>



namespace ogl
{

Mesh::
Mesh(
	Type             const type,
	std::string_view const file
	)
{
	reset_transforms();
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
	scale       = glm::vec3(1.0F);
	position    = glm::vec3(0.0F);
	orientation = glm::quat();
}

auto Mesh::
rotate(
	float     const angle,
	glm::vec3 const axis
	)
	-> void
{
	auto const a = glm::radians(angle / 2.0F);
	rotate(glm::quat(
		axis.x * sinf(a),
		axis.y * sinf(a),
		axis.z * sinf(a),
		cosf(a)));
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
translate_matrix(
	) const
	-> glm::mat4
{
	return glm::translate(glm::mat4(1.0F), position);
}

auto Mesh::
rotate_matrix(
	) const
	-> glm::mat4
{
	return glm::mat4_cast(orientation);
}

auto Mesh::
scale_matrix(
	) const
	-> glm::mat4
{
	return glm::scale(glm::mat4(1.0F), scale);
}

auto Mesh::
model_matrix(
	) const
	-> glm::mat4
{
	return
		translate_matrix()
		* rotate_matrix()
		* scale_matrix();
}

auto Mesh::
normal_matrix(
	) const
	-> glm::mat3
{
	return glm::transpose(glm::inverse(
		glm::mat3(rotate_matrix())
		* glm::mat3(scale_matrix())));
}



// Load model or file
auto Mesh::
load(
	Type             const type,
	std::string_view const file
	)
	-> bool
{
	type_ = type;
	switch (type)
	{
		case Empty:
		{
			clean();
			break;
		}

		case File:
		{
			if (!obj_.load(file))
			{
				std::cerr << "ERROR: Could not load " <<
					file << std::endl;
				return false;
			}
			initialise_obj_mesh();
			break;
		}

		case Other:
		{
			std::cerr << "ERROR: Cannot create Mesh of type 'Other'" << std::endl;
			return false;
		}

		default:
		{
			std::function<geometry::Geometry()>
			const static geom_of[]
			{
				[Triangle] = geometry::triangle,
				[Quad    ] = geometry::quad,
				[Cube    ] = geometry::cube
			};
			auto const [p, n, t] = geom_of[type]();
			initialise_mesh(p, n, t);
			break;
		}
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
		std::cerr <<
			"ERROR: Vertex and Normal count is unequal" << std::endl;
		return;
	}

	// Make sure there are vertices to use
	size_ = vertices.size();
	if (size_ == 0)
	{
		std::cerr << "ERROR: No vertices supplied" << std::endl;
		return;
	}

	if (type_ == Empty)
		type_ = Other;

	// Make sure obj has vertices for bounds calculation
	if (obj_.vertices.empty())
	{
		for (auto i = 0U; i < size_; ++i)
		{
			auto v = obj::Vertex();
			v.position = vertices[i];
			v.normal   = normals[i];
			// Might not have uvs
			if (!uvs.empty())
				v.uv       = uvs[i];
			obj_.vertices.push_back(v);
		}
	}

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
	reset_transforms();
	shader.reset();
}



// Initialise
auto Mesh::
initialise_obj_mesh(
	)
	-> void
{
	size_ = obj_.vertices.size();

	// Get positions and normals
	auto positions = std::vector<glm::vec3>();
	auto normals   = std::vector<glm::vec3>();
	auto uvs       = std::vector<glm::vec2>();

	positions.reserve(size_);
	normals.reserve(size_);
	uvs.reserve(size_);

	for (auto const& v : obj_.vertices)
	{
		positions.push_back(v.position);
		normals.push_back(v.normal);
		uvs.push_back(v.uv);
	}

	initialise_mesh(positions, normals, uvs);
}

} // namespace ogl
