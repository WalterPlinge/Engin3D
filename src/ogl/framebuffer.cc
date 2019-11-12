#include <e3d/ogl/framebuffer.hh>

#include <vector>

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
	height_(height)
{
	glGenFramebuffers(1, &id_);
}

Framebuffer::
~Framebuffer(
	)
{
	glDeleteFramebuffers(1, &id_);
	id_ = 0;
}



// Detail
auto Framebuffer::
id(
	) const
	-> GLuint
{
	return id_;
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



// Behaviour
auto Framebuffer::
bind(
	) const
	-> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, id_);
}

auto Framebuffer::
unbind(
	) const
	-> void
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto Framebuffer::
is_valid(
	) const
	-> bool
{
	return id_ != 0;
}



// Attachments
auto Framebuffer::
frames(
	) const
	-> std::unordered_map<std::size_t, Texture> const&
{
	return colour_;
}

auto Framebuffer::
frame(
	std::size_t index
	) const
	-> std::optional<Texture const*>
{
	if (auto const f = colour_.find(index); f != colour_.end())
		return &f->second;
	return std::nullopt;
}

auto Framebuffer::
depth(
	) const
	-> std::optional<Texture> const&
{
	return depth_;
}

//auto Framebuffer::
//stencil(
//	) const
//	-> std::optional<Texture> const&
//{
//	return stencil_;
//}



// Add attachments
auto Framebuffer::
add_frame(
	std::size_t index,
	GLint       internal_format,
	GLenum      data_format,
	GLenum      data_type
	)
	-> void
{
	bind();

	glActiveTexture(GL_TEXTURE0);

	auto tex = Texture(width_, height_);
	glBindTexture(tex.type(), tex.id());
	glTexImage2D(
		tex.type(),
		0,
		internal_format,
		tex.width(),
		tex.height(),
		0,
		data_format,
		data_type,
		nullptr);

	glTexParameteri(tex.type(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(tex.type(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(tex.type(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(tex.type(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, id_);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0 + index,
		tex.type(),
		tex.id(),
		0);

	colour_.insert(std::pair(index, tex));

	unbind();

	update_draw_buffers();
}

auto Framebuffer::
add_depth(
	)
	-> void
{
	bind();

	glActiveTexture(GL_TEXTURE0);

	auto tex = Texture(width_, height_);
	glBindTexture(tex.type(), tex.id());
	glTexImage2D(
		tex.type(),
		0,
		GL_DEPTH_COMPONENT,
		tex.width(),
		tex.height(),
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		nullptr);

	glTexParameteri(tex.type(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(tex.type(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(tex.type(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(tex.type(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterfv(tex.type(), GL_TEXTURE_BORDER_COLOR,
		glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));

	glBindFramebuffer(GL_FRAMEBUFFER, id_);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		tex.type(),
		tex.id(),
		0);

	depth_ = tex;

	unbind();
}



// Remove attachments
auto Framebuffer::
remove_frame(
	std::size_t index
	)
	-> void
{
	colour_.erase(index);

	update_draw_buffers();
}

auto Framebuffer::
remove_depth(
	)
	-> void
{
	depth_ = std::nullopt;
}



// Update buffer with current attachments
auto Framebuffer::
update_draw_buffers(
	) const
	-> void
{
	bind();

	// Set draw buffer
	auto draw_buffers = std::vector<GLenum>();
	draw_buffers.reserve(colour_.size());
	for (auto const& c : colour_)
		draw_buffers.push_back(GL_COLOR_ATTACHMENT0 + c.first);
	glDrawBuffers(draw_buffers.size(), draw_buffers.data());

	unbind();
}

} // namespace ogl
