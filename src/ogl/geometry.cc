#include <e3d/ogl/geometry.hh>

namespace
ogl
{

namespace
geometry
{

auto
triangle(
	)
	-> Geometry
{
	auto const p = Positions
	{
		glm::vec3( 0.0F, 0.0F,  1.0F),
		glm::vec3(-1.0F, 0.0F, -1.0F),
		glm::vec3( 1.0F, 0.0F, -1.0F)
	};

	auto const n = Normals
	{
		glm::vec3(0.0F, -1.0F, 0.0F),
		glm::vec3(0.0F, -1.0F, 0.0F),
		glm::vec3(0.0F, -1.0F, 0.0F)
	};

	auto const t = TextureUVs
	{
		glm::vec2(0.5F, 1.0F),
		glm::vec2(0.0F, 0.0F),
		glm::vec2(1.0F, 0.0F)
	};

	return std::tie(p, n, t);
}

auto
quad(
	)
	-> Geometry
{
	auto const p = Positions
	{
		glm::vec3( 1.0F,  1.0F, 0.0F),
		glm::vec3(-1.0F,  1.0F, 0.0F),
		glm::vec3(-1.0F, -1.0F, 0.0F),
		glm::vec3(-1.0F, -1.0F, 0.0F),
		glm::vec3( 1.0F, -1.0F, 0.0F),
		glm::vec3( 1.0F,  1.0F, 0.0F),
	};

	auto const n = Normals
	{
		glm::vec3(0.0F, 0.0F, 1.0F),
		glm::vec3(0.0F, 0.0F, 1.0F),
		glm::vec3(0.0F, 0.0F, 1.0F),
		glm::vec3(0.0F, 0.0F, 1.0F),
		glm::vec3(0.0F, 0.0F, 1.0F),
		glm::vec3(0.0F, 0.0F, 1.0F)
	};

	auto const t = TextureUVs
	{
		glm::vec2(1.0F, 1.0F),
		glm::vec2(0.0F, 1.0F),
		glm::vec2(0.0F, 0.0F),
		glm::vec2(0.0F, 0.0F),
		glm::vec2(1.0F, 0.0F),
		glm::vec2(1.0F, 1.0F)
	};

	return std::tie(p, n, t);
}

auto
cube(
	)
	-> Geometry
{

	auto const p = Positions
	{
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

	auto const n = Normals
	{
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

	auto const t = TextureUVs
	{
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

	return std::tie(p, n, t);
}

} // namespace geometry

} // namespace ogl
