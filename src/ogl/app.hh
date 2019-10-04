#ifndef OGL_APP_H
#define OGL_APP_H

///////////////////////////////////////////////////////////////
// HEADERS

// GLM
#include <glm/vec2.hpp>

// GLEW
// ReSharper disable once CppUnusedIncludeDirective
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Project
#include "camera.hh"
#include "mesh.hh"

///////////////////////////////////////////////////////////////
// RENDER NAMESPACE

namespace ogl {

	///////////////////////////////////////////////////////////
	// APP CLASS

	class App {
	public:

		///////////////////////////////////////////////////////
		// GETTERS

		static Camera& get_camera();

		///////////////////////////////////////////////////////
		// INITIALISE

		static void initialise_renderer();

		///////////////////////////////////////////////////////
		// UPDATE

		static void update(const float& dt);
		static bool key_pressed(const int& key);
		static void show_fps(const float& newTime);

		///////////////////////////////////////////////////////
		// RENDER

		static void clear();
		static void draw(const Mesh& mesh);
		static void display();

		///////////////////////////////////////////////////////
		// CLEANUP

		static bool should_close();
		static void close();

	private:

		///////////////////////////////////////////////////////
		// CALLBACK FUNCTIONS

		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
		static void mouse_callback(GLFWwindow* window, double x_position, double y_position);
		static void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

		///////////////////////////////////////////////////////
		// ATTRIBUTES

		// Window handle
		static GLFWwindow* window_;

		// Camera
		static Camera camera_;

		// Dimensions
		static constexpr unsigned height = 600;
		static constexpr unsigned width = 800;
		static int screen_height_;
		static int screen_width_;

		// Input
		static bool first_mouse_;
		static bool keyboard_[1024];
		static glm::vec2 mouse_pos_;
	};

}

#endif
