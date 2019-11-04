#include <e3d/obj/obj.hh>

#include <algorithm>
#include <fstream>
#include <sstream>
#include <utility>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

namespace obj
{

auto static
split_string(
	std::string_view str,
	char const       d
	)
	-> std::vector<std::string_view>
{
	auto tok = std::vector<std::string_view>();
	tok.reserve(std::count(str.begin(), str.end(), d) + 1);
	for (auto l = 0ULL; l != str.npos; str = str.substr(l + 1, str.npos))
	{
		if ((l = str.find(d)) && !str.empty())
		{
			tok.push_back(str.substr(0, l));
		}
	}
	tok.shrink_to_fit();
	return tok;
}

template <class T>
auto static
get_element(
	std::vector<T> const& list,
	std::intmax_t  const& index
	)
	-> T
{
	// Index relative to list end
	if (index < 0)
		return list[list.size() + index];

	if (index == 0)
		return list[0];

	// Index relative to 0
	return list[index - 1];
}

auto static
in_triangle(
	Vertex const& p,
	Vertex const& a,
	Vertex const& b,
	Vertex const& c
	)
	-> bool
{
	// Vectors
	auto const v0 = c.position - a.position;
	auto const v1 = b.position - a.position;
	auto const v2 = p.position - a.position;

	// Dot products
	auto const dot00 = glm::dot(v0, v0);
	auto const dot01 = glm::dot(v0, v1);
	auto const dot02 = glm::dot(v0, v2);
	auto const dot11 = glm::dot(v1, v1);
	auto const dot12 = glm::dot(v1, v2);

	// Barycentric coordinates
	auto const inv_denom = 1.0F / dot00 * dot11 - dot01 * dot01;
	auto const u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
	auto const v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

	// Check if point is in triangle
	return u >= 0 && v >= 0 && u + v < 1;
}

auto static
generate_face_vertices(
	std::vector<std::string_view> const& face_tokens,
	std::vector<glm::vec3>        const& positions,
	std::vector<glm::vec3>        const& normals,
	std::vector<glm::vec2>        const& uvs
	)
	-> std::vector<Vertex>
{
	auto vertices = std::vector<Vertex>();

	// Keep track of missing normals
	auto generate_normals = false;

	// Parse every face
	for (auto i = 1U; i < face_tokens.size(); ++i)
	{
		// Split vertex tokens
		auto const tokens = split_string(face_tokens[i], '/');

		auto vertex = Vertex{};

		// Vertex always has position
		vertex.position = get_element(
			positions,
			std::stoi(std::string(tokens[0])));

		// Texture coordinates (optional)
		if (tokens.size() > 1)
			if (!tokens[1].empty())
				vertex.uv = get_element(
					uvs,
					std::stoi(std::string(tokens[1])));

		// Normals
		if (tokens.size() > 2)
			vertex.normal = get_element(
				normals,
				std::stoi(std::string(tokens[2])));
		else
			generate_normals = true;

		vertices.push_back(vertex);
	}

	// Generate missing normals
	if (generate_normals && vertices.size() >= 3)
		for (Vertex& vertex : vertices)
			vertex.normal = glm::cross(
				vertices[0].position - vertices[1].position,
				vertices[2].position - vertices[1].position);

	return vertices;
}

auto
triangulate_vertices(
	std::vector<Vertex> const& vertices
	)
	-> std::vector<std::size_t>
{
	// Already a triangle
	if (vertices.size() == 3)
		return { 0, 1, 2 };

	// Indices and copy of vertices
	auto indices = std::vector<std::size_t>();
	auto verts   = vertices;

	do
	{
		for (auto i = 0U; i < verts.size(); ++i)
		{
			// Previous, current and next
			auto const& previous = verts[(!i ? verts.size() : i) - 1];
			auto const& current  = verts[i];
			auto const& next     = verts[(i + 1) == verts.size() ? 0 : i + 1];

			// Only triangle left
			if (verts.size() == 3)
			{
				for (auto j = 0U; j < vertices.size(); ++j)
				{
					auto const& pos = vertices[j].position;
					if (pos == previous.position
					 || pos == current.position
					 || pos == next.position)
						indices.push_back(j);
				}

				return indices;
			}

			// Skip exterior vertex
			auto const theta = glm::degrees(glm::angle(
				previous.position - current.position,
				next.position - current.position));
			if (theta <= 0 || theta >= 180)
				continue;

			// If any vertices are within this triangle
			auto vertex_in_triangle = false;
			for (auto const& v : vertices)
			{
				if (v.position != previous.position
				 && v.position != current.position
				 && v.position != next.position
				 && in_triangle(v, previous, current, next))
				{
					vertex_in_triangle = true;
					break;
				}
			}

			if (vertex_in_triangle)
				continue;

			// Create triangle
			for (auto j = 0U; j < vertices.size(); ++j)
			{
				auto const& pos = vertices[j].position;
				if (pos == previous.position
				|| pos == current.position
				|| pos == next.position)
					indices.push_back(j);
			}

			// Delete current from list
			for (auto j = 0U; j < verts.size(); ++j)
			{
				if (verts[j].position == current.position)
				{
					verts.erase(verts.begin() + j);
					break;
				}
			}

			// Reset i to -1 for loop increment
			i = -1;
		}
	} while (!indices.empty() && !verts.empty());

	return indices;
}



auto Obj::
load(
	std::string_view const filename
	)
	-> bool
{
	// If the file is not an .obj file return false
	if (filename.substr(filename.size() - 4, 4) != ".obj")
		return false;

	// Clear previous data
	clear();

	// Load file lines into string vector
	auto lines = std::vector<std::string>();
	{
		std::ifstream file(filename.data());
		if (!file.good())
			return false;

		for (std::string line; std::getline(file, line); /**/)
			lines.push_back(line);
	}



	// Temporary lists
	auto temp_positions = std::vector<glm::vec3>();
	auto temp_normals   = std::vector<glm::vec3>();
	auto temp_uvs       = std::vector<glm::vec2>();

	auto temp_vertices = std::vector<Vertex>();
	auto temp_indices  = std::vector<std::size_t>();

	// Parse tokens for each line
	for (std::string_view const& line : lines)
	{
		auto const tokens = split_string(line, ' ');

		// Skip empty lines
		if (tokens.empty())
			continue;

		// Vertex position
		else if (tokens.front() == "v")
		{
			temp_positions.emplace_back(
				std::stof(std::string(tokens[1])),
				std::stof(std::string(tokens[2])),
				std::stof(std::string(tokens[3])));
		}

		// Vertex normal
		else if (tokens.front() == "vn")
		{
			temp_normals.emplace_back(
				std::stof(std::string(tokens[1])),
				std::stof(std::string(tokens[2])),
				std::stof(std::string(tokens[3])));
		}

		// Vertex texture coordinates
		else if (tokens.front() == "vt")
		{
			temp_uvs.emplace_back(
				std::stof(std::string(tokens[1])),
				std::stof(std::string(tokens[2])));
		}

		// Face vertices
		else if (tokens.front() == "f")
		{
			// Face vertices and indices
			auto const face_vertices = generate_face_vertices(
				tokens,
				temp_positions,
				temp_normals,
				temp_uvs);
			auto const face_indices = triangulate_vertices(face_vertices);

			// Add vertices to both lists
			temp_vertices.insert(temp_vertices.end(), face_vertices.begin(), face_vertices.end());
			vertices.insert(vertices.end(), face_vertices.begin(), face_vertices.end());

			// Add indices to both lists
			for (auto const& i : face_indices)
			{
				temp_indices.push_back(temp_vertices.size() + face_vertices.size() + i);
				indices.push_back(vertices.size() - face_vertices.size() + i);
			}
		}
	}

	// Deal with last mesh
	if (!temp_indices.empty() && !temp_vertices.empty())
	{
		auto mesh = Mesh();
		mesh.name = filename;
		mesh.material = Material();
		mesh.vertices = temp_vertices;
		mesh.indices = temp_indices;
		meshes.push_back(mesh);
	}

	return !meshes.empty() && !vertices.empty() && !indices.empty();
}

auto Obj::
clear(
	)
	-> void
{
	meshes.clear();
	vertices.clear();
	indices.clear();
	materials.clear();
}

} // namespace obj
