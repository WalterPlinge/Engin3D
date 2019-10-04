#ifndef OBJ_H
#define OBJ_H
#pragma once

///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <string>
#include <vector>

// GLM
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

///////////////////////////////////////////////////////////////
// OBJ NAMESPACE

namespace obj {

	///////////////////////////////////////////////////////////
	// VERTEX

	struct Vertex {
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 normal = glm::vec3(0.0f);
		glm::vec2 uv = glm::vec2(0.0f);
		bool operator==(Vertex const& other) { return position == other.position && normal == other.normal && uv == other.uv; }
	};

	///////////////////////////////////////////////////////////
	// MATERIAL

	struct Material {

		// ATTRIBUTES
		std::string name;

		// COLOURS
		glm::vec3 ambient_colour = glm::vec3(0.0f);
		glm::vec3 diffuse_colour = glm::vec3(0.0f);
		glm::vec3 specular_colour = glm::vec3(0.0f);

		// LIGHTING
		float specular_exponent = 0;
		float optical_density = 0;
		float dissolve = 0;
		int illumination = 0;

		// MAPS
		std::string ambient_texture_map;
		std::string diffuse_texture_map;
		std::string specular_texture_map;
		std::string specular_highlight_map;
		std::string alpha_texture_map;
		std::string bump_map;
	};

	///////////////////////////////////////////////////////////
	// MESH

	struct Mesh {

		// ATTRIBUTES
		std::string name;

		// MATERIAL
		Material material;

		// VERTICES
		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;
	};

	///////////////////////////////////////////////////////////
	// OBJECT

	class Obj {
	public:

		///////////////////////////////////////////////////////
		// GETTERS

		std::vector<Mesh> get_meshes() const;
		std::vector<Vertex> get_vertices() const;
		std::vector<unsigned> get_indices() const;
		std::vector<Material> get_materials() const;

		///////////////////////////////////////////////////////
		// FILE LOADING

		bool load(const std::string& filename);

		///////////////////////////////////////////////////////
		// CLEANUP

		void clear();

	private:

		///////////////////////////////////////////////////////
		// STRING OPERATIONS

		std::vector<std::string> split_string(const std::string& string, const char& delimiter) const;

		///////////////////////////////////////////////////////
		// VECTOR OPERATIONS

		template <class T>
		static T get_element(const std::vector<T>& list, const int& index);

		///////////////////////////////////////////////////////
		// VERTEX OPERATIONS

		template <glm::length_t L, typename T>
		bool in_triangle(const glm::vec<L, T>& p, const glm::vec<L, T>& a, const glm::vec<L, T>& b, const glm::vec<L, T>& c);
		std::vector<Vertex> generate_face_vertices(const std::vector<std::string>& face_tokens, const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs) const;
		std::vector<unsigned> triangulate_vertices(const std::vector<Vertex>& vertices);

		///////////////////////////////////////////////////////
		// LOADED DATA

		std::vector<Mesh> meshes_;
		std::vector<Vertex> vertices_;
		std::vector<unsigned> indices_;
		std::vector<Material> materials_;
	};
}

#endif
