#pragma once

#include <memory>
#include <optional>
#include <unordered_map>

#define GLEW_STATIC
#include <GL/glew.h>

#include <e3d/ogl/texture.hh>

namespace ogl
{

class Framebuffer
{
	GLuint id_;
	GLuint width_;
	GLuint height_;

	std::unordered_map<std::size_t, std::unique_ptr<Texture>> colour_;
	std::optional<std::unique_ptr<Texture>>                   depth_;
///	std::optional<std::unique_ptr<Texture>>                   stencil_;

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



	// Behaviour
	auto
	bind(
		) const
		-> void;

	auto
	unbind(
		) const
		-> void;

	auto
	is_valid(
		) const
		-> bool;



	// Attachments
	auto
	frames(
		) const
		-> std::unordered_map<std::size_t, std::unique_ptr<Texture>> const&;

	auto
	frame(
		std::size_t index
		) const
		-> std::optional<Texture const*>;

	auto
	depth(
		) const
		-> std::optional<std::unique_ptr<Texture>> const&;

///	auto
///	stencil(
///		) const
///		-> std::optional<std::unique_ptr<Texture>> const&;



	// Add attachments
	auto
	add_frame(
		std::size_t index,
		GLint       internal_format,
		GLenum      data_format,
		GLenum      data_type
		)
		-> void;

	auto
	add_depth(
		)
		-> void;

///	auto
///	add_stencil(
///		)
///		-> void;



	// Remove attachments
	auto
	remove_frame(
		std::size_t index
		)
		-> void;

	auto
	remove_depth(
		)
		-> void;

///	auto
///	remove_stencil(
///		)
///		-> void;

private:

	// Update buffer with current attachments
	auto
	update_draw_buffers(
		) const
		-> void;
};

} // namespace ogl
