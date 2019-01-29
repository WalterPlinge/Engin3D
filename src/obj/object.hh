#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H
#pragma once

///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <string>
#include <vector>

// GLM
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

// Print progress to console while loading (large models)
#define OBJL_CONSOLE_OUTPUT

///////////////////////////////////////////////////////////////
// OBJ NAMESPACE

namespace object {

	///////////////////////////////////////////////////////////
	// ALGORITHM NAMESPACE

	namespace algorithm {

		///////////////////////////////////////////////////////
		// TRIANGLES

		// A test to see if p1 is on the same side as p2 of a line segment ab
		inline bool same_side(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& a, const glm::vec3& b);

		// Generate a cross produect normal for a triangle
		inline glm::vec3 triangle_normal(const glm::vec3& t1, const glm::vec3& t2, const glm::vec3& t3);

		// Check to see if a vec3 point is within a 3 vec3 triangle
		inline bool in_triangle(const glm::vec3& point, const glm::vec3& tri1, const glm::vec3& tri2, const glm::vec3& tri3);

		///////////////////////////////////////////////////////
		// TOKENS

		// Split a string into a string array at a given token
		inline void split(const std::string& in, std::vector<std::string>& out, const std::string& token);

		// Get tail of string after first token and possibly following spaces
		inline std::string tail(const std::string& in);

		// Get first token of string
		inline std::string first_token(const std::string& in);

		// Get element at given index position
		template <class T>
		const T& get_element(const std::vector<T>& elements, std::string& index);
	}

	///////////////////////////////////////////////////////////
	// VERTEX

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	///////////////////////////////////////////////////////////
	// MATERIAL

	struct Material {

		///////////////////////////////////////////////////////
		// CONSTRUCTOR

		Material();

		///////////////////////////////////////////////////////
		// ATTRIBUTES

		std::string name;

		///////////////////////////////////////////////////////
		// COLOURS

		glm::vec3 ambient_colour;
		glm::vec3 diffuse_colour;
		glm::vec3 specular_colour;

		///////////////////////////////////////////////////////
		// LIGHTING

		float specular_exponent;
		float optical_density;
		float dissolve;
		int illumination;
		
		///////////////////////////////////////////////////////
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
		
		///////////////////////////////////////////////////////
		// CONSTRUCTORS

		Mesh();
		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned>& indices);

		///////////////////////////////////////////////////////
		// ATTRIBUTES

		std::string name;

		///////////////////////////////////////////////////////
		// MATERIAL

		Material material;

		///////////////////////////////////////////////////////
		// VERTICES

		std::vector<Vertex> vertices;
		std::vector<unsigned> indices;

	};

	///////////////////////////////////////////////////////////
	// OBJECT

	class Object {
	public:

		///////////////////////////////////////////////////////
		// CONSTRUCTORS

		Object();
		Object(Object&&) noexcept;
		Object(const Object&);

		~Object();

		///////////////////////////////////////////////////////
		// OPERATORS

		Object& operator=(Object&&) noexcept;
		Object& operator=(const Object&);

		///////////////////////////////////////////////////////
		// FILE LOADING
		
		bool load_file(const std::string& filename);
		bool load_materials(const std::string& filename);

		///////////////////////////////////////////////////////
		// ATTRIBUTES

		// Loaded Mesh Objects
		std::vector<Mesh> loaded_meshes;
		// Loaded Vertex Objects
		std::vector<Vertex> loaded_vertices;
		// Loaded Index Positions
		std::vector<unsigned int> loaded_indices;
		// Loaded Material Objects
		std::vector<Material> loaded_materials;

	private:

		///////////////////////////////////////////////////////
		// VERTEX OPERATIONS

		// Generate vertices from a list of positions, normals, uvs and a face line
		void vertices_from_obj(std::vector<Vertex>& o_verts, const std::vector<glm::vec3>& i_positions, const std::vector<glm::vec2>& i_t_coords, const std::vector<glm::vec3>& i_normals, const std::string& i_line) const;
		// Triangulate a list of vertices into a face by printing indicies corresponding with triangles within it
		void vertex_triangluation(std::vector<unsigned int>& o_indices, const std::vector<Vertex>& i_verts) const;
	};
}

#endif
