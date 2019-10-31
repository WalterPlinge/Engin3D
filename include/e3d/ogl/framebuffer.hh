#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <e3d/ogl/texture.hh>

namespace ogl
{

class Framebuffer
{
	GLuint buffer_;
	GLuint width_;
	GLuint height_;
	Texture frame_;
	Texture depth_;

public:

	// Constructor
	Framebuffer(
		GLuint width,
		GLuint height
		);

	~Framebuffer(
		);

	// Details
	auto
	buffer(
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
	frame(
		) const
		-> Texture const&;

	auto
	depth(
		) const
		-> Texture const&;

	// Loading and clearing
	auto
	create(
		)
		-> bool;

	auto
	clear(
		)
		-> void;
};

} // namespace ogl
