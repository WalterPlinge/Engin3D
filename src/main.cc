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
	app::camera.position = glm::vec3(0.0F, -20.0F, 5.0F);
	app::camera.look_at(glm::vec3(0.0F));
	app::camera.sensitivity = 0.001F;



	// Generate shader
	auto lambert = std::make_shared<Shader>("Lambert");
	lambert->add(GL_VERTEX_SHADER, "res/shaders/lambert.vert");
	lambert->add(GL_FRAGMENT_SHADER, "res/shaders/lambert.frag");
	lambert->build();



	// Mesh
	auto ground = Mesh(Mesh::Quad);
	ground.scale(glm::vec3(5.0F));
	ground.shader = lambert;

	auto cube = Mesh(Mesh::Cube);
	cube.translate(glm::vec3(1.0F, 0.0F, 1.0F));
	cube.shader = lambert;

	auto sphere = Mesh("res/models/sphere.obj");
	sphere.translate(glm::vec3(-1.0F, 0.0F, 1.0F));
	sphere.shader = lambert;



	// Time variables
	auto           time        = 0.0F;
	auto           accumulator = 0.0F;
	auto constexpr framerate   = 60.0F;
	auto const     delta_time   = 1.0F / framerate;
	auto           current_time = float(glfwGetTime());



	// Application loop
	while (!app::should_close()) {

		// Update time
		auto       new_time  = float(glfwGetTime());
		auto const frameTime = new_time - current_time;
		current_time         = new_time;
		accumulator         += frameTime;



		// Application update
		app::update(frameTime);
		app::show_fps(new_time);



		// Physics
		while (accumulator >= delta_time) {

			// Time
			accumulator -= delta_time;
			time        += delta_time;
		}



		// Render
		app::clear();
		app::draw(ground);
		app::draw(cube);
		app::draw(sphere);
		app::display();
	}



	// Close app
	app::close();
	exit(EXIT_SUCCESS);
}
