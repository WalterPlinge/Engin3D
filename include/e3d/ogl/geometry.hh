#pragma once

#include <tuple>
#include <vector>

#include <glm/glm.hpp>

namespace
ogl
{

namespace
geometry
{

using Positions  = std::vector<glm::vec3>;
using Normals    = std::vector<glm::vec3>;
using TextureUVs = std::vector<glm::vec2>;

using Geometry = std::tuple<Positions, Normals, TextureUVs>;

auto
triangle(
	)
	-> Geometry;

auto
quad(
	)
	-> Geometry;

auto
cube(
	)
	-> Geometry;

} // namespace geometry

} // namespace ogl
