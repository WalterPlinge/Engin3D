#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <e3d/ogl/camera.hh>
#include <e3d/ogl/framebuffer.hh>
#include <e3d/ogl/mesh.hh>
#include <e3d/ogl/texture.hh>

namespace ogl::renderer
{

// Types
using clock      = std::chrono::high_resolution_clock;
using time_point = std::chrono::time_point<clock>;
using duration   = std::chrono::duration<float>;

// Keyboard keys are either pressed or not
using keyboard_t = std::array<bool, GLFW_KEY_LAST>;



// Details
std::string    extern title;
std::uintmax_t extern width;
std::uintmax_t extern height;

// Camera
Camera extern camera;



// Details
auto
resolution(
	)
	-> glm::vec2;

auto
keyboard(
	)
	-> const keyboard_t&;

auto
mouse_pos(
	)
	-> glm::vec2;

auto
clear_colour(
	glm::vec4 colour
	)
	-> void;

auto
is_running(
	)
	-> bool;



// Initialise
auto
start(
	)
	-> void;

auto
update(
	time_point new_time
	)
	-> void;



// Render
auto
bind(
	Texture const& texture,
	GLuint         index
	)
	-> void;

auto
target(
	Framebuffer const* framebuffer = nullptr
	)
	-> void;

auto
clear(
	)
	-> void;

auto
draw(
	Mesh const& mesh
	)
	-> void;

auto
display(
	)
	-> void;



// Shutdown
auto
shutdown(
	)
	-> void;

} // namespace ogl::renderer
