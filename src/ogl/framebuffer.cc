#include <e3d/ogl/framebuffer.hh>

#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ogl
{

// Constructors
Framebuffer::
Framebuffer(
	GLuint width,
	GLuint height
	) :
	width_(width),
	height_(height),
	frame_(Texture(width, height)),
	depth_(Texture(width, height))
{
	glActiveTexture(GL_TEXTURE0);

	// Create frame image data
	glBindTexture(frame_.type(), frame_.id());
	glTexImage2D(
		frame_.type(),
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		nullptr);

	glTexParameteri(frame_.type(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(frame_.type(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(frame_.type(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(frame_.type(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Create depth image data
	glBindTexture(depth_.type(), depth_.id());
	glTexImage2D(
		depth_.type(),
		0,
		GL_DEPTH_COMPONENT,
		width,
		height,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		nullptr);

	glTexParameteri(depth_.type(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(depth_.type(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(depth_.type(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(depth_.type(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterfv(depth_.type(), GL_TEXTURE_BORDER_COLOR,
		glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));

	// Create frame buffer and attach textures
	glGenFramebuffers(1, &buffer_);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer_);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frame_.type(), frame_.id(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_.type(), depth_.id(), 0);

	// Set draw buffer
	static GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &draw_buffer);

	// Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::
~Framebuffer(
	)
{
	glDeleteFramebuffers(1, &buffer_);
	buffer_ = 0;
}



// Detail
auto Framebuffer::
buffer(
	) const
	-> GLuint
{
	return buffer_;
}

auto Framebuffer::
width(
	) const
	-> GLuint
{
	return width_;
}

auto Framebuffer::
height(
	) const
	-> GLuint
{
	return height_;
}

auto Framebuffer::
frame(
	) const
	-> Texture const&
{
	return frame_;
}

auto Framebuffer::
depth(
	) const
	-> Texture const&
{
	return depth_;
}

auto Framebuffer::
is_valid(
	) const
	-> bool
{
	return buffer_ != 0;
}

} // namespace ogl
