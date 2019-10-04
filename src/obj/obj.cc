#include "obj.hh"

///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <fstream>
#include <sstream>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

///////////////////////////////////////////////////////////////
// OBJ NAMESPACE

namespace obj {

	///////////////////////////////////////////////////////
	// GETTERS

	std::vector<Mesh> Obj::get_meshes() const
	{
		return meshes_;
	}

	std::vector<Vertex> Obj::get_vertices() const
	{
		return vertices_;
	}

	std::vector<unsigned> Obj::get_indices() const
	{
		return indices_;
	}

	std::vector<Material> Obj::get_materials() const
	{
		return materials_;
	}

	///////////////////////////////////////////////////////////
	// FILE LOADING

	bool Obj::load(const std::string& filename)
	{
		// If the file is not an .obj file return false
		if (filename.substr(filename.size() - 4, 4) != ".obj")
			return false;

		// Clear previous data
		clear();

		// Open file to string vector
		std::ifstream file(filename);
		if (!file)
			return false;

		std::vector<std::string> lines;
		for (std::string line; std::getline(file, line); )
			lines.push_back(line);

		file.close();

		// Temporary lists
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;

		// Parse tokens for each line
		for (const std::string& line : lines) {
			const std::vector<std::string> line_tokens(split_string(line, ' '));

			// Vertex position
			if (line_tokens.front() == "v") {
				positions.emplace_back(std::stof(line_tokens[1]), std::stof(line_tokens[2]), std::stof(line_tokens[3]));
			}
			// Vertex normal
			else if (line_tokens.front() == "vn") {
				normals.emplace_back(std::stof(line_tokens[1]), std::stof(line_tokens[2]), std::stof(line_tokens[3]));
			}
			// Vertex texture coordinates
			else if (line_tokens.front() == "vt") {
				uvs.emplace_back(std::stof(line_tokens[1]), std::stof(line_tokens[2]));
			}
			// Face vertices
			else if (line_tokens.front() == "f") {

				// Face vertices and indices
				std::vector<Vertex> face_vertices(generate_face_vertices(line_tokens, positions, normals, uvs));
				std::vector<unsigned> face_indices(triangulate_vertices(face_vertices));

				// Add vertices to both lists
				vertices.insert(vertices.end(), face_vertices.begin(), face_vertices.end());
				vertices_.insert(vertices_.end(), face_vertices.begin(), face_vertices.end());

				// Add indices to both lists
				for (const unsigned& i : face_indices) {
					indices.push_back(vertices.size() + face_vertices.size() + i);
					indices_.push_back(vertices_.size() - face_vertices.size() + i);
				}
			}
		}

		// Deal with last mesh
		if (!indices.empty() && !vertices.empty()) {
			meshes_.push_back({ filename, Material(), vertices, indices });
		}

		return !meshes_.empty() && !vertices_.empty() && !indices_.empty();
	}

	///////////////////////////////////////////////////////
	// CLEANUP

	void Obj::clear()
	{
		meshes_.clear();
		vertices_.clear();
		indices_.clear();
		materials_.clear();
	}

	///////////////////////////////////////////////////////
	// STRING OPERATIONS

	std::vector<std::string> Obj::split_string(const std::string& string, const char& delimiter) const
	{
		std::vector<std::string> tokens;
		std::stringstream string_stream(string);

		for (std::string token; std::getline(string_stream, token, delimiter); )
			tokens.push_back(token);

		return tokens;
	}

	///////////////////////////////////////////////////////
	// VECTOR OPERATIONS

	template <class T>
	T Obj::get_element(const std::vector<T>& list, const int& index)
	{
		// Index relative to list end
		if (index < 0)
			return list[list.size() + index];

		// Index relative to 0
		return list[index - 1];
	}

	///////////////////////////////////////////////////////
	// VERTEX OPERATIONS

	template <glm::length_t L, typename T>
	bool Obj::in_triangle(const glm::vec<L, T>& p, const glm::vec<L, T>& a, const glm::vec<L, T>& b, const glm::vec<L, T>& c)
	{
		// Vectors
		const glm::vec<L, T> v0(c - a);
		const glm::vec<L, T> v1(b - a);
		const glm::vec<L, T> v2(p - a);

		// Dot products
		const float dot00(glm::dot(v0, v0));
		const float dot01(glm::dot(v0, v1));
		const float dot02(glm::dot(v0, v2));
		const float dot11(glm::dot(v1, v1));
		const float dot12(glm::dot(v1, v2));

		// Barycentric coordinates
		const float inv_denom(1 / dot00 * dot11 - dot01 * dot01);
		const float u((dot11 * dot02 - dot01 * dot12) * inv_denom);
		const float v((dot00 * dot12 - dot01 * dot02) * inv_denom);

		// Check if point is in triangle
		return u >= 0 && v >= 0 && u + v < 1;
	}

	std::vector<Vertex> Obj::generate_face_vertices(const std::vector<std::string>& face_tokens, const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs) const
	{
		std::vector<Vertex> vertices;

		// Keep track of missing normals
		bool generate_normals(false);

		// Parse every vertex
		for (unsigned i(1); i < face_tokens.size(); ++i) {

			// Split vertex tokens
			std::vector<std::string> vertex_tokens(split_string(face_tokens[i], '/'));

			// Vertex always has position
			Vertex vertex{
				get_element(positions, std::stoi(vertex_tokens[0]))
			};

			// Texture coordinates (optional)
			if (vertex_tokens.size() > 1)
				if (!vertex_tokens[1].empty())
					vertex.uv = get_element(uvs, std::stoi(vertex_tokens[1]));

			// Normals
			if (vertex_tokens.size() > 2)
				vertex.normal = get_element(normals, std::stoi(vertex_tokens[2]));
			else
				generate_normals = true;

			vertices.push_back(vertex);
		}

		// Generate missing normals
		if (generate_normals && vertices.size() >= 3)
			for (Vertex& vertex : vertices)
				vertex.normal = glm::cross(vertices[0].position - vertices[1].position, vertices[2].position - vertices[1].position);

		return vertices;
	}

	std::vector<unsigned> Obj::triangulate_vertices(const std::vector<Vertex>& vertices)
	{
		// Already a triangle
		if (vertices.size() == 3)
			return std::vector<unsigned>({ 0, 1, 2 });

		// Indices and copy of vertices
		std::vector<unsigned> indices;
		std::vector<Vertex> verts(vertices);
		do {
			for (unsigned i(0); i < verts.size(); ++i) {

				// Previous, current and next
				const Vertex& previous(verts[(!i ? verts.size() : i) - 1]);
				const Vertex& current(verts[i]);
				const Vertex& next(verts[i + 1 == verts.size() ? 0 : i + 1]);

				// Only triangle left
				if (verts.size() == 3) {
					for (unsigned j(0); j < vertices.size(); ++j) {
						if (vertices[j].position == previous.position)
							indices.push_back(j);

						if (vertices[j].position == current.position)
							indices.push_back(j);

						if (vertices[j].position == next.position)
							indices.push_back(j);
					}

					return indices;
				}

				// Skip exterior vertex
				const float theta(glm::angle(previous.position - current.position, next.position - current.position) * (180 / glm::pi<float>()));
				if (theta <= 0 || theta >= 180)
					continue;

				// If any vertices are within this triangle
				bool vertex_in_triangle(false);
				for (const Vertex& v : vertices)
					if (v.position != previous.position && v.position != current.position && v.position != next.position && in_triangle(v.position, previous.position, current.position, next.position)) {
						vertex_in_triangle = true;
						break;
					}
				if (vertex_in_triangle)
					continue;

				// Create triangle
				for (unsigned j(0); j < vertices.size(); ++j) {
					if (vertices[j].position == previous.position)
						indices.push_back(j);

					if (vertices[j].position == current.position)
						indices.push_back(j);

					if (vertices[j].position == next.position)
						indices.push_back(j);
				}

				// Delete current from list
				for (unsigned j(0); j < verts.size(); ++j) {
					if (verts[j].position == current.position) {
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
}
