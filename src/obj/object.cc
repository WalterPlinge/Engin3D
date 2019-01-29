#include <obj/object.hh>

///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <fstream>
#include <iostream>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/projection.hpp>

///////////////////////////////////////////////////////////////
// OBJ NAMESPACE

namespace object {

	///////////////////////////////////////////////////////////
	// ALGORITHM NAMESPACE

	namespace algorithm {

		///////////////////////////////////////////////////////
		// TRIANGLES

		// A test to see if p1 is on the same side as p2 of a line segment ab
		bool same_side(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& a, const glm::vec3& b)
		{
			return glm::dot(glm::cross(b - a, p1 - a), glm::cross(b - a, p2 - a)) >= 0;
		}

		// Generate a cross produect normal for a triangle
		glm::vec3 triangle_normal(const glm::vec3& t1, const glm::vec3& t2, const glm::vec3& t3)
		{
			return glm::cross(t2 - t1, t3 - t1);
		}

		// Check to see if a vec3 point is within a 3 vec3 triangle
		bool in_triangle(const glm::vec3& point, const glm::vec3& tri1, const glm::vec3& tri2, const glm::vec3& tri3)
		{
			// If it isn't it will never be on the triangle
			if (!same_side(point, tri1, tri2, tri3) || !same_side(point, tri2, tri1, tri3) || !same_side(point, tri3, tri1, tri2))
				return false;

			// If the distance from the triangle to the point is 0 it lies on the triangle
			return glm::length(glm::proj(point, triangle_normal(tri1, tri2, tri3))) == 0;
		}

		///////////////////////////////////////////////////////
		// TOKENS

		// Split a string into a string array at a given token
		void split(const std::string& in, std::vector<std::string>& out, const std::string& token)
		{
			out.clear();
			std::string temp;
			for (unsigned i = 0; i < in.size(); i++) {
				auto test = in.substr(i, token.size());
				if (test == token) {
					if (!temp.empty()) {
						out.push_back(temp);
						temp.clear();
						i += token.size() - 1;
					} else {
						out.emplace_back("");
					}
				} else if (i + token.size() >= in.size()) {
					temp += in.substr(i, token.size());
					out.push_back(temp);
					break;
				} else {
					temp += in[i];
				}
			}
		}

		// Get tail of string after first token and possibly following spaces
		std::string tail(const std::string& in)
		{
			const size_t& token_start(in.find_first_not_of(" \t"));
			const size_t& space_start(in.find_first_of(" \t", token_start));
			const size_t& tail_start(in.find_first_not_of(" \t", space_start));
			const size_t& tail_end(in.find_last_not_of(" \t"));

			if (tail_start != std::string::npos && tail_end != std::string::npos)
				return in.substr(tail_start, tail_end - tail_start + 1);

			if (tail_start != std::string::npos)
				return in.substr(tail_start);

			return "";
		}

		// Get first token of string
		std::string first_token(const std::string& in)
		{
			if (!in.empty()) {

				const size_t& token_start(in.find_first_not_of(" \t"));
				const size_t& token_end(in.find_first_of(" \t", token_start));

				if (token_start != std::string::npos && token_end != std::string::npos)
					return in.substr(token_start, token_end - token_start);

				if (token_start != std::string::npos)
					return in.substr(token_start);
			}

			return "";
		}

		// Get element at given index position
		template <class T>
		const T& get_element(const std::vector<T>& elements, std::string& index)
		{
			int i(std::stoi(index));

			if (i < 0)
				i = int(elements.size()) + i;
			else
				i--;

			return elements[i];
		}
	}

	///////////////////////////////////////////////////////////
	// MATERIAL

	///////////////////////////////////////////////////////////
	// CONSTRUCTOR

	Material::Material() : ambient_colour(), diffuse_colour(), specular_colour(), specular_exponent(0), optical_density(0), dissolve(0), illumination(0) {}

	///////////////////////////////////////////////////////////
	// MESH

	///////////////////////////////////////////////////////////
	// CONSTRUCTORS

	Mesh::Mesh() = default;
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned>& indices) : vertices(vertices), indices(indices) {}

	///////////////////////////////////////////////////////////
	// OBJECT

	///////////////////////////////////////////////////////////
	// CONSTRUCTORS

	Object::Object() = default;
	Object::Object(Object&&) noexcept = default;
	Object::Object(const Object&) = default;

	Object::~Object()
	{
		loaded_meshes.clear();
		loaded_vertices.clear();
		loaded_indices.clear();
		loaded_materials.clear();
	}

	///////////////////////////////////////////////////////////
	// OPERATORS

	Object& Object::operator=(Object&&) noexcept = default;
	Object& Object::operator=(const Object&) = default;

	///////////////////////////////////////////////////////////
	// FILE LOADING

	bool Object::load_file(const std::string& filename)
	{
		// If the file is not an .obj file return false
		if (filename.substr(filename.size() - 4, 4) != ".obj")
			return false;

		// Clear meshes
		loaded_meshes.clear();
		loaded_vertices.clear();
		loaded_indices.clear();

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<Vertex>	vertices;
		std::vector<unsigned> indices;
		std::vector<std::string> mesh_mat_names;
		bool listening(false);
		std::string	mesh_name;
		Mesh temp_mesh;

#ifdef OBJL_CONSOLE_OUTPUT
		const unsigned int output_every_nth(1000);
		auto output_indicator(output_every_nth);
#endif

		// Open file and put lines in string vector
		std::ifstream file(filename);
		if (!file.is_open())
			return false;

		const std::string content{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
		file.close();

		std::vector<std::string> lines;
		algorithm::split(content, lines, "\n");

		for (const std::string& line : lines){

#ifdef OBJL_CONSOLE_OUTPUT
			if ((output_indicator = (output_indicator + 1) % output_every_nth) == 1)
				if (!mesh_name.empty())
					std::cout << "\r- " << mesh_name << "\t| vertices > " << positions.size() << "\t| texcoords > " << uvs.size() << "\t| normals > " << normals.size() << "\t| triangles > " << (vertices.size() / 3) << (!mesh_mat_names.empty() ? "\t| material: " + mesh_mat_names.back() : "");
#endif

			// Generate a mesh object or prepare for an object to be created
			if (algorithm::first_token(line) == "o" || algorithm::first_token(line) == "g" || line[0] == 'g') {
				if (!listening) {
					listening = true;
					if (algorithm::first_token(line) == "o" || algorithm::first_token(line) == "g")
						mesh_name = algorithm::tail(line);
					else
						mesh_name = "unnamed";

				} else {

					// Generate the mesh to put into the array
					if (!indices.empty() && !vertices.empty()) {

						// Create mesh
						temp_mesh = Mesh(vertices, indices);
						temp_mesh.name = mesh_name;

						// Insert mesh
						loaded_meshes.push_back(temp_mesh);

						// Cleanup
						vertices.clear();
						indices.clear();
						mesh_name.clear();
						mesh_name = algorithm::tail(line);

					} else {
						if (algorithm::first_token(line) == "o" || algorithm::first_token(line) == "g")
							mesh_name = algorithm::tail(line);
						else
							mesh_name = "unnamed";
					}
				}

#ifdef OBJL_CONSOLE_OUTPUT
				std::cout << std::endl;
				output_indicator = 0;
#endif

			}
			// Generate a vertex position
			else if (algorithm::first_token(line) == "v") {
				std::vector<std::string> spos;
				glm::vec3 vpos;

				algorithm::split(algorithm::tail(line), spos, " ");

				vpos.x = stof(spos[0]);
				vpos.y = stof(spos[1]);
				vpos.z = stof(spos[2]);

				positions.push_back(vpos);
			}
			// Generate a vertex texture coordinate
			else if (algorithm::first_token(line) == "vt") {
				std::vector<std::string> stex;
				glm::vec2 vtex;

				algorithm::split(algorithm::tail(line), stex, " ");

				vtex.x = stof(stex[0]);
				vtex.y = stof(stex[1]);

				uvs.push_back(vtex);
			}
			// Generate a vertex normal
			else if (algorithm::first_token(line) == "vn") {
				std::vector<std::string> snor;
				glm::vec3 vnor;

				algorithm::split(algorithm::tail(line), snor, " ");

				vnor.x = stof(snor[0]);
				vnor.y = stof(snor[1]);
				vnor.z = stof(snor[2]);

				normals.push_back(vnor);
			}
			// Generate a face (vertices & indices)
			else if (algorithm::first_token(line) == "f") {

				// Generate the vertices
				std::vector<Vertex> v_verts;
				vertices_from_obj(v_verts, positions, uvs, normals, line);

				// Add vertices
				for (const Vertex& v_vert : v_verts) {
					vertices.push_back(v_vert);
					loaded_vertices.push_back(v_vert);
				}

				std::vector<unsigned> i_indices;
				vertex_triangluation(i_indices, v_verts);

				// Add indices
				for (const unsigned& i_index : i_indices) {
					unsigned ind_num(static_cast<unsigned int>(vertices.size() - v_verts.size()) + i_index);
					indices.push_back(ind_num);

					ind_num = static_cast<unsigned int>(loaded_vertices.size() - v_verts.size()) + i_index;
					loaded_indices.push_back(ind_num);
				}
			}
			// Get mesh material name
			else if (algorithm::first_token(line) == "usemtl") {
				mesh_mat_names.push_back(algorithm::tail(line));

				// Create new mesh, if material changes within a group
				if (!indices.empty() && !vertices.empty()) {

					// Create mesh
					temp_mesh = Mesh(vertices, indices);
					temp_mesh.name = mesh_name;
					int i(1);
					while (true) {
						++i;
						temp_mesh.name = mesh_name + "_" + std::to_string(i);

						bool found(false);
						for (Mesh& mesh : loaded_meshes)
							if (mesh.name == temp_mesh.name) {
								found = true;
								break;
							}

						if (!found)
							break;
					}
					
					// Insert mesh
					loaded_meshes.push_back(temp_mesh);

					// Cleanup
					vertices.clear();
					indices.clear();
				}

#ifdef OBJL_CONSOLE_OUTPUT
				output_indicator = 0;
#endif

			}
			// Load materials
			else if (algorithm::first_token(line) == "mtllib") {

				// Generate a path to the material file
				std::vector<std::string> temp;
				algorithm::split(filename, temp, "/");
				std::string path_to_mat;

				if (temp.size() != 1)
					for (auto i = 0; i < int(temp.size()) - 1; i++)
						path_to_mat += temp[i] + "/";

				path_to_mat += algorithm::tail(line);

#ifdef OBJL_CONSOLE_OUTPUT
				std::cout << std::endl << "- find materials in: " << path_to_mat << std::endl;
#endif

				// Load materials
				load_materials(path_to_mat);
			}
		}

#ifdef OBJL_CONSOLE_OUTPUT
		std::cout << std::endl;
#endif

		// Deal with last mesh
		if (!indices.empty() && !vertices.empty()) {

			// Create mesh
			temp_mesh = Mesh(vertices, indices);
			temp_mesh.name = mesh_name;

			// Insert mesh
			loaded_meshes.push_back(temp_mesh);
		}

		// Set materials for each mesh
		for (int i = 0; i < int(mesh_mat_names.size()); i++) {
			std::string mat_name = mesh_mat_names[i];

			// Find corresponding material name in loaded materials when found copy material variables into mesh material
			for (auto& loadedMaterial : loaded_materials) {
				if (loadedMaterial.name == mat_name) {
					loaded_meshes[i].material = loadedMaterial;
					break;
				}
			}
		}
		return !(loaded_meshes.empty() && loaded_vertices.empty() && loaded_indices.empty());
	}

	bool Object::load_materials(const std::string& filename)
	{
		// If the file is not a material file return false
		if (filename.substr(filename.size() - 4, filename.size()) != ".mtl")
			return false;

		// Open file and put lines in string vector
		std::ifstream file(filename);
		if (!file.is_open())
			return false;

		const std::string content{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
		file.close();

		std::vector<std::string> lines;
		algorithm::split(content, lines, "\n");

		Material temp_material;
		auto listening = false;

		// Go through each line looking for material variables
		for (const std::string& line : lines) {

			// new material and material name
			if (algorithm::first_token(line) == "newmtl") {
				if (!listening) {
					listening = true;
					if (line.size() > 7)
						temp_material.name = algorithm::tail(line);
					else
						temp_material.name = "none";

				} else {

					// Push Back loaded Material
					loaded_materials.push_back(temp_material);

					// Clear Loaded Material
					temp_material = Material();
					if (line.size() > 7)
						temp_material.name = algorithm::tail(line);
					else
						temp_material.name = "none";
				}
			}

			// Ambient Color
			if (algorithm::first_token(line) == "Ka") {
				std::vector<std::string> temp;
				algorithm::split(algorithm::tail(line), temp, " ");

				if (temp.size() != 3)
					continue;

				temp_material.ambient_colour.x = stof(temp[0]);
				temp_material.ambient_colour.y = stof(temp[1]);
				temp_material.ambient_colour.z = stof(temp[2]);
			}

			// Diffuse Color
			if (algorithm::first_token(line) == "Kd") {
				std::vector<std::string> temp;
				algorithm::split(algorithm::tail(line), temp, " ");

				if (temp.size() != 3)
					continue;

				temp_material.diffuse_colour.x = stof(temp[0]);
				temp_material.diffuse_colour.y = stof(temp[1]);
				temp_material.diffuse_colour.z = stof(temp[2]);
			}

			// Specular Color
			if (algorithm::first_token(line) == "Ks") {
				std::vector<std::string> temp;
				algorithm::split(algorithm::tail(line), temp, " ");

				if (temp.size() != 3)
					continue;

				temp_material.specular_colour.x = stof(temp[0]);
				temp_material.specular_colour.y = stof(temp[1]);
				temp_material.specular_colour.z = stof(temp[2]);
			}

			// Specular Exponent
			if (algorithm::first_token(line) == "Ns")
				temp_material.specular_exponent = stof(algorithm::tail(line));

			// Optical Density
			if (algorithm::first_token(line) == "Ni")
				temp_material.optical_density = stof(algorithm::tail(line));

			// Dissolve
			if (algorithm::first_token(line) == "d")
				temp_material.dissolve = stof(algorithm::tail(line));

			// Illumination
			if (algorithm::first_token(line) == "illum")
				temp_material.illumination = stoi(algorithm::tail(line));

			// Ambient Texture Map
			if (algorithm::first_token(line) == "map_Ka")
				temp_material.ambient_texture_map = algorithm::tail(line);

			// Diffuse Texture Map
			if (algorithm::first_token(line) == "map_Kd")
				temp_material.diffuse_texture_map = algorithm::tail(line);

			// Specular Texture Map
			if (algorithm::first_token(line) == "map_Ks")
				temp_material.specular_texture_map = algorithm::tail(line);

			// Specular Hightlight Map
			if (algorithm::first_token(line) == "map_Ns")
				temp_material.specular_highlight_map = algorithm::tail(line);

			// Alpha Texture Map
			if (algorithm::first_token(line) == "map_d")
				temp_material.alpha_texture_map = algorithm::tail(line);

			// Bump Map
			if (algorithm::first_token(line) == "map_Bump" || algorithm::first_token(line) == "map_bump" || algorithm::first_token(line) == "bump")
				temp_material.bump_map = algorithm::tail(line);
		}

		// Push Back loaded Material
		loaded_materials.push_back(temp_material);

		// Test to see if anything was loaded if not return false
		return !loaded_materials.empty();
	}

	///////////////////////////////////////////////////////////
	// VERTEX OPERATIONS

	// Generate vertices from a list of positions, normals, uvs and a face line
	void Object::vertices_from_obj(std::vector<Vertex>& o_verts, const std::vector<glm::vec3>& i_positions, const std::vector<glm::vec2>& i_t_coords, const std::vector<glm::vec3>& i_normals, const std::string& i_line) const
	{
		std::vector<std::string> s_face;
		std::vector<std::string> s_vert;
		Vertex v_vert{};
		algorithm::split(algorithm::tail(i_line), s_face, " ");
		auto no_normal(false);

		// For every given vertex
		for (const std::string& i : s_face) {

			// See what type the vertex is
			auto v_type = 0;
			algorithm::split(i, s_vert, "/");

			// Check for just position - v1
			if (s_vert.size() == 1)
				v_type = 1;

			// Check for position & texture - v1/vt1
			else if (s_vert.size() == 2)
				v_type = 2;

			// Check for position, texture and normal - v1/vt1/vn1 or if position and normal - v1//vn1
			else if (s_vert.size() == 3)
				v_type = !s_vert[1].empty() ? 4 : 3;

			// Calculate and store the vertex
			switch (v_type) {

				// P
				case 1: 
				{
					v_vert.position = algorithm::get_element(i_positions, s_vert[0]);
					v_vert.uv = glm::vec2(0, 0);
					no_normal = true;
					o_verts.push_back(v_vert);
					break;
				}

				// P/T
				case 2: 
				{
					v_vert.position = algorithm::get_element(i_positions, s_vert[0]);
					v_vert.uv = algorithm::get_element(i_t_coords, s_vert[1]);
					no_normal = true;
					o_verts.push_back(v_vert);
					break;
				}

				// P//N
				case 3: 
				{
					v_vert.position = algorithm::get_element(i_positions, s_vert[0]);
					v_vert.uv = glm::vec2(0, 0);
					v_vert.normal = algorithm::get_element(i_normals, s_vert[2]);
					o_verts.push_back(v_vert);
					break;
				}

				// P/T/N
				case 4: 
				{
					v_vert.position = algorithm::get_element(i_positions, s_vert[0]);
					v_vert.uv = algorithm::get_element(i_t_coords, s_vert[1]);
					v_vert.normal = algorithm::get_element(i_normals, s_vert[2]);
					o_verts.push_back(v_vert);
					break;
				}

				default:
					break;
			}
		}
		
		// Take care of missing normals these may not be truly acurate but it is the best they get for not compiling a mesh with normals
		if (no_normal)
			for (Vertex& oVert : o_verts)
				oVert.normal = glm::cross(o_verts[0].position - o_verts[1].position, o_verts[2].position - o_verts[1].position);
	}

	// Triangulate a list of vertices into a face by printing indicies corresponding with triangles within it
	void Object::vertex_triangluation(std::vector<unsigned>& o_indices, const std::vector<Vertex>& i_verts) const
	{
		// If there are 2 or less verts, no triangle can be created, so exit
		if (i_verts.size() < 3)
			return;

		// If it is a triangle no need to calculate it
		if (i_verts.size() == 3) {
			o_indices.push_back(0);
			o_indices.push_back(1);
			o_indices.push_back(2);
			return;
		}
		
		// Create a list of vertices
		std::vector<Vertex> t_verts(i_verts);
		while (true) {

			// For every vertex
			for (int i = 0; i < int(t_verts.size()); i++) {

				// The previous vertex in the list
				const Vertex& p_prev(t_verts[(i == 0 ? t_verts.size() : i) - 1]);

				// The current vertex;
				const Vertex& p_cur(t_verts[i]);
				
				// pNext = the next vertex in the list
				const Vertex& p_next(t_verts[(i == int(t_verts.size()) - 1 ? 0 : i + 1)]);

				// Check to see if there are only 3 verts left if so this is the last triangle
				if (t_verts.size() == 3) {

					// Create a triangle from p_cur, p_prev, p_next
					for (int j = 0; j < int(i_verts.size()); j++) {
						if (i_verts[j].position == p_cur.position) 
							o_indices.push_back(j);

						if (i_verts[j].position == p_prev.position) 
							o_indices.push_back(j);

						if (i_verts[j].position == p_next.position) 
							o_indices.push_back(j);
					}

					t_verts.clear();
					break;
				}

				if (t_verts.size() == 4) {

					// Create a triangle from p_cur, p_prev, p_next
					for (int j = 0; j < int(i_verts.size()); j++) {
						if (i_verts[j].position == p_cur.position) {
							o_indices.push_back(j);
						}
						if (i_verts[j].position == p_prev.position) {
							o_indices.push_back(j);
						}
						if (i_verts[j].position == p_next.position) {
							o_indices.push_back(j);
						}
					}

					glm::vec3 temp_vec;
					for (Vertex& t_vert : t_verts) {
						if (t_vert.position != p_cur.position && t_vert.position != p_prev.position && t_vert.position != p_next.position) {
							temp_vec = t_vert.position;
							break;
						}
					}

					// Create a triangle from p_cur, p_prev, p_next
					for (auto j = 0; j < int(i_verts.size()); j++) {
						if (i_verts[j].position == p_prev.position) {
							o_indices.push_back(j);
						}
						if (i_verts[j].position == p_next.position) {
							o_indices.push_back(j);
						}
						if (i_verts[j].position == temp_vec) {
							o_indices.push_back(j);
						}
					}

					t_verts.clear();
					break;
				}

				// If vertex is not an interior vertex
				const auto& theta(glm::angle(p_prev.position - p_cur.position, p_next.position - p_cur.position) * (180 / glm::pi<float>()));
				if (theta <= 0 && theta >= 180)
					continue;

				// If any vertices are within this triangle
				auto in_tri(false);
				for (const Vertex& i_vert : i_verts) {
					if (algorithm::in_triangle(i_vert.position, p_prev.position, p_cur.position, p_next.position) && i_vert.position != p_prev.position && i_vert.position != p_cur.position && i_vert.position != p_next.position) {
						in_tri = true;
						break;
					}
				}

				if (in_tri)
					continue;

				// Create a triangle from p_cur, p_prev, p_next
				for (int j = 0; j < int(i_verts.size()); j++) {
					if (i_verts[j].position == p_cur.position) {
						o_indices.push_back(j);
					}
					if (i_verts[j].position == p_prev.position) {
						o_indices.push_back(j);
					}
					if (i_verts[j].position == p_next.position) {
						o_indices.push_back(j);
					}
				}

				// Delete p_cur from the list
				for (int j = 0; j < int(t_verts.size()); j++) {
					if (t_verts[j].position == p_cur.position) {
						t_verts.erase(t_verts.begin() + j);
						break;
					}
				}

				// reset i to the start, -1 since loop will add 1 to it
				i = -1;
			}                               
			
			// if no triangles were created
			if (o_indices.empty())
				break;

			// if no more vertices
			if (t_verts.empty())
				break;
		}
	}
}
