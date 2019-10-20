#pragma once

#include <array>
#include <cstdint>
#include <functional>

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



// Details
std::string                extern title;
std::uintmax_t             extern width;
std::uintmax_t             extern height;

// Camera
Camera                     extern camera;

// Functions
std::function<void()>      extern setup_function;
std::function<void()>      extern input_function;
std::function<void(float)> extern update_function;
std::function<void(float)> extern fixed_update_function;
std::function<void()>      extern render_function;
std::function<void()>      extern close_function;



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



// Run application
auto
run(
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
close(
	)
	-> void;

} // namespace ogl::app
