#include <e3d/ogl/texture.hh>



namespace ogl
{

Texture::
Texture(
	GLenum target,
	GLint  format,
	GLenum unit
	)
	: id_(0U),
	  unit_(unit),
	  format_(format),
	  target_(target)
{
	glGenTextures(1, &id_);
	glBindTexture(target, id_);
	glEnable(target);
}

Texture::
~Texture()
{
	glDeleteTextures(1, &id_);
}

auto Texture::
id(
	) const
	-> GLuint
{
	return id_;
}

auto Texture::
unit(
	) const
	-> GLenum
{
	return unit_;
}

auto Texture::
format(
	) const
	-> GLint
{
	return format_;
}

auto Texture::
target(
	) const
	-> GLenum
{
	return target_;
}

auto Texture::
bind(
	) const
	-> void
{
	glActiveTexture(unit_);
	glBindTexture(target_, id_);
}



// Texture 1D
Texture1D::
Texture1D()
	: Texture(GL_TEXTURE_1D)
{}

Texture1D::
Texture1D(
	GLint  format,
	GLenum unit
	) :
	Texture(GL_TEXTURE_1D, format, unit)
{}

Texture1D::
~Texture1D()
{}

auto Texture1D::
generate(
	GLvoid const* data,
	GLenum        data_format,
	GLenum        type
	) const
	-> void
{
	bind();
	glTexImage1D(target_, 0, format_, width, 0, data_format, type, data);
}



// Texture2D
Texture2D::
Texture2D()
	: Texture(GL_TEXTURE_2D)
{}

Texture2D::
Texture2D(
	GLint  format,
	GLenum unit
	)
	: Texture(GL_TEXTURE_2D, format, unit)
{}

Texture2D::
~Texture2D()
{}

auto Texture2D::
generate(
	GLvoid const* data,
	GLenum        data_format,
	GLenum        type
	) const
	-> void
{
	bind();
	glTexImage2D(target_, 0, format_, width, height, 0, data_format, type, data);
}

} // namespace ogl
