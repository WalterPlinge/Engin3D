#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace ogl
{

class Texture
{
	GLuint id_;
	GLuint width_;
	GLuint height_;
	GLenum type_;

public:

	// Constructors
	Texture(
		GLuint width,
		GLuint height
		);

	~Texture(
		);

	// Details
	auto
	id(
		) const
		-> GLuint;

	auto
	width(
		) const
		-> GLuint;

	auto
	height(
		) const
		-> GLuint;

	auto
	type(
		) const
		-> GLenum;
};

} // namespace ogl
