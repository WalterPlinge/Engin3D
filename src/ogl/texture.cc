#include <e3d/ogl/texture.hh>

namespace ogl
{

// Constructors
Texture::
Texture(
	GLuint const width,
	GLuint const height
	) :
	width_(width),
	height_(height),
	type_(GL_TEXTURE_2D)
{
	glGenTextures(1, &id_);
}

Texture::
~Texture(
	)
{
	glDeleteTextures(1, &id_);
}



// Details
auto Texture::
id(
	) const
	-> GLuint
{
	return id_;
}

auto Texture::
width(
	) const
	-> GLuint
{
	return width_;
}

auto Texture::
height(
	) const
	-> GLuint
{
	return height_;
}

auto Texture::
type(
	) const
	-> GLenum
{
	return type_;
}

} // namespace ogl
