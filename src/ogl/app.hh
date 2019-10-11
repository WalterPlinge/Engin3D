#pragma once

#include <array>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "camera.hh"
#include "mesh.hh"



namespace ogl::app
{

// Keyboard array shorthand
using keyboard_t = std::array<bool, GLFW_KEY_LAST>;



// Camera
Camera extern camera;



// Initialise
auto
initialise_renderer(
	)
	-> void;



// Update
auto
update(
	float delta_time
	)
	-> void;

auto
show_fps(
	float new_time
	)
	-> void;



// Render
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
should_close(
	)
	-> bool;

auto
close(
	)
	-> void;



// Utilities
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

} // namespace ogl::app
