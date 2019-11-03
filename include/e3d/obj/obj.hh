#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace obj
{

struct Vertex
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 normal   = glm::vec3(0.0f);
	glm::vec2 uv       = glm::vec2(0.0f);
};

struct Material
{
	std::string name;

	glm::vec3 ambient_colour  = glm::vec3(0.0f);
	glm::vec3 diffuse_colour  = glm::vec3(0.0f);
	glm::vec3 specular_colour = glm::vec3(0.0f);

	float         specular_exponent = 0;
	float         optical_density   = 0;
	float         dissolve          = 0;
	std::intmax_t illumination      = 0;

	std::string ambient_texture_map;
	std::string diffuse_texture_map;
	std::string specular_texture_map;
	std::string specular_highlight_map;
	std::string alpha_texture_map;
	std::string bump_map;
};

struct Mesh
{
	std::string              name;
	Material                 material;
	std::vector<Vertex>      vertices;
	std::vector<std::size_t> indices;
};

struct Obj
{
	std::vector<Mesh>        meshes;
	std::vector<Material>    materials;
	std::vector<Vertex>      vertices;
	std::vector<std::size_t> indices;

	auto
	load(
		std::string_view filename
		)
		-> bool;

	auto
	clear(
		)
		-> void;
};

} // namespace obj
