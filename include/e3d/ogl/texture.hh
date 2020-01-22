#pragma once

// STD
#include <iostream>
#include <typeinfo>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

namespace ogl
{

class Texture
{
protected:

	GLuint id_;
	GLenum unit_; // @OpenGL: Does the texture need to store the unit?
	GLint  format_;
	GLenum target_;

public:

	Texture(
		GLenum target = GL_TEXTURE_1D,
		GLint  format = GL_RGBA32F,
		GLenum unit   = GL_TEXTURE0
		);

	virtual
	~Texture(
		);



	// Data
	auto
	id(
		) const
		-> GLuint;

	auto
	unit(
		) const
		-> GLenum;

	auto
	format(
		) const
		-> GLint;

	auto
	target(
		) const
		-> GLenum;


	// Bind texture to context
	auto
	bind(
		) const
		-> void;

	// Generate texture from data
	auto virtual
	generate(
		GLvoid const* data,
		GLenum        data_format = GL_RGBA,
		GLenum        type        = GL_FLOAT
		) const
		-> void
		= 0;

	// Set a texture parameter
	template <typename T>
	auto
	parameter(
		GLenum p,
		T      v
		) const
		-> void
	{
		bind();

		if constexpr (std::is_same<T, GLint>::value)
			glTexParameteri(target_, p, v);
		else if constexpr (std::is_same<T, GLfloat>::value)
			glTexParameterf(target_, p, v);
		else
			std::cerr << "ERROR: Invalid texture parameter type " <<
				typeid(T).name() << std::endl;
	}
};



class Texture1D final :
	public Texture
{
public:

	GLsizei width;



	Texture1D(
		);

	Texture1D(
		GLint  format = GL_RGBA32F,
		GLenum unit   = GL_TEXTURE0
		);

	virtual
	~Texture1D(
		)
		override final;



	auto virtual
	generate(
		GLvoid const* data,
		GLenum        data_format = GL_RGBA,
		GLenum        type        = GL_FLOAT
		) const
		-> void
		override final;
};



class Texture2D final :
	public Texture
{
public:

	GLsizei width;
	GLsizei height;



	Texture2D(
		);

	Texture2D(
		GLint  format = GL_RGBA32F,
		GLenum unit   = GL_TEXTURE0
		);

	virtual
	~Texture2D(
		)
		override final;

	auto virtual
	generate(
		GLvoid const* data,
		GLenum        data_format = GL_RGBA,
		GLenum        type        = GL_FLOAT
		) const
		-> void
		override final;
};

} // namespace ogl
