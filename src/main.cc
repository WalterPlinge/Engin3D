#include <iostream>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include "ogl/app.hh"



using namespace ogl;



auto
main(
	)
	-> int
{
	// Application setup

	app::initialise_renderer();
	app::camera.position(glm::vec3(5.0F));
	app::camera.look_at(glm::vec3(0.0F));

	// Generate shader
	auto shader = ogl::Shader();
	shader.name = "basic shader";
	shader.add(GL_VERTEX_SHADER, "res/shaders/solid.vert");
	shader.add(GL_FRAGMENT_SHADER, "res/shaders/solid_green.frag");
	shader.build();

	////////////////////////////////////////////////////////////
	// Mesh

	auto ground = ogl::Mesh();
//	ground.position(glm::vec3(15.0f, 0.0f, 15.0f));
//	ground.scale(glm::vec3(15.0f));
	ground.shader = shader;
///	Mesh sphereMesh("resources/models/sphere.obj");
///	sphereMesh.shader = stripe;

	// Random number generator
///	std::mt19937 rng;
///	rng.seed(std::random_device()());

	////////////////////////////////////////////////////////////
	// Time variables

	auto           time        = 0.0F;
	auto           accumulator = 0.0F;
	auto constexpr framerate   = 60.0F;
	auto const     delta_time   = 1.0F / framerate;
	auto           current_time = float(glfwGetTime());

	////////////////////////////////////////////////////////////
	// Application loop

	while (!app::should_close()) {

		// Update time
		auto       new_time  = float(glfwGetTime());
		auto const frameTime = new_time - current_time;
		current_time         = new_time;
		accumulator         += frameTime;

		/////////////////////////////////////////////////////////
		// Application update

		app::update(frameTime);
		app::show_fps(new_time);

		/////////////////////////////////////////////////////////
		// Physics

		while (accumulator >= delta_time) {

			//////////////////////////////////////////////////////
			// Time

			accumulator -= delta_time;
			time        += delta_time;
		}

		/////////////////////////////////////////////////////////
		// Render

		app::clear();
		app::draw(ground);
		app::display();
	}

	////////////////////////////////////////////////////////////
	// Close app

	app::close();
	exit(EXIT_SUCCESS);
}
