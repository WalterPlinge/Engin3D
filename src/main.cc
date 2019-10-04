///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <iostream>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

// Project
#include "ogl/app.hh"
using ogl::App;

///////////////////////////////////////////////////////////////
// PROGRAM ENTRY

int main() {

	////////////////////////////////////////////////////////////
	// TEST AREA

	// TEST CAMEERA
	// TEST SHADER
	// TEST OBJ
	// TEST MESH
	// TEST APP

	////////////////////////////////////////////////////////////
	// Application setup

	App::initialise_renderer();
///	App::get_camera() = glm::vec3(0.0f);

	////////////////////////////////////////////////////////////
	// Generate shader

///	const Shader red("resources/shaders/solid.vert", "resources/shaders/solid_red.frag");
///	const Shader green("resources/shaders/solid.vert", "resources/shaders/solid_green.frag");
///	const Shader lambert("resources/shaders/lambert.vert", "resources/shaders/lambert.frag");
///	const Shader stripe("resources/shaders/stripe.vert", "resources/shaders/stripe.frag");

	////////////////////////////////////////////////////////////
	// Mesh

///	Mesh ground;
///	ground.SetPos(glm::vec3(15.0f, 0.0f, 15.0f));
///	ground.Scale(glm::vec3(15.0f));
///	ground.shader = green;
///	Mesh sphereMesh("resources/models/sphere.obj");
///	sphereMesh.shader = stripe;

	// Random number generator
///	std::mt19937 rng;
///	rng.seed(std::random_device()());

	////////////////////////////////////////////////////////////
	// Time variables

	float time(0.0f);
	float accumulator(0.0f);
	constexpr float framerate(60.0f);
	const float	deltaTime(1.0f / framerate);
	float currentTime(glfwGetTime());

	////////////////////////////////////////////////////////////
	// Application loop

	while (!App::should_close()) {
		
		// Update time
		const float newTime(glfwGetTime());
		const float frameTime(newTime - currentTime);
		currentTime = newTime;
		accumulator += frameTime;

		/////////////////////////////////////////////////////////
		// Application update

		App::update(frameTime);
		App::show_fps(newTime);

		/////////////////////////////////////////////////////////
		// Physics

		while (accumulator >= deltaTime) {

			//////////////////////////////////////////////////////
			// Time

			accumulator -= deltaTime;
			time += deltaTime;
		}

		/////////////////////////////////////////////////////////
		// Render

		App::clear();
///		App::draw(ground);
		App::display();
	}

	////////////////////////////////////////////////////////////
	// Close app

	App::close();
	exit(EXIT_SUCCESS);
}
