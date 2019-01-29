#include "app.hh"

///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <cassert>
#include <iostream>
#include <sstream>

// GLM
#include <glm/gtc/type_ptr.hpp>

///////////////////////////////////////////////////////////////
// RENDER NAMESPACE

namespace ogl {

	///////////////////////////////////////////////////////////
	// GETTERS

	Camera& App::get_camera()
	{
		return camera_;
	}

	///////////////////////////////////////////////////////////
	// INITIALISE

	void App::initialise_renderer()
	{
		// Set close function to run at exit
		std::atexit(close);

		// Initialise GLFW
		if (!glfwInit()) {
			std::cerr << "ERROR -> APP -> GLFW -> INITIALISE" << std::endl;
			exit(EXIT_FAILURE);
		}

		// GLFW settings (v3.3, core profile, not resizable, forward compatability)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		// Window error handling
		window_ = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr);

		if (!window_) {
			std::cerr << "ERROR -> APP -> GLFW -> WINDOW -> CREATE" << std::endl;
			close();
			exit(EXIT_FAILURE);
		}

		// Set context and framebuffer size
		glfwMakeContextCurrent(window_);
		glfwGetFramebufferSize(window_, &screen_width_, &screen_height_);

		// Set callback functions
		glfwSetKeyCallback(window_, key_callback);
		glfwSetCursorPosCallback(window_, mouse_callback);
		glfwSetScrollCallback(window_, scroll_callback);

		// Remove mouse cursor
		glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Modern GLEW approach
		glewExperimental = GL_TRUE;

		// Initialise GLEW to setup the opengl function pointers
		if (glewInit() != GLEW_OK) {
			std::cout << "ERROR -> APP -> GLEW -> INITIALISE" << std::endl;
			close();
			exit(EXIT_FAILURE);
		}

		// Define the viewport dimensions
		glViewport(0, 0, screen_width_, screen_height_);

		// Set up some other opengl options
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// App variables
		camera_.set_aspect(screen_width_, screen_height_);

		std::cout << "App initialised" << std::endl;
	}

	///////////////////////////////////////////////////////
	// UPDATE

	void App::update(const float& dt)
	{
		glfwPollEvents();

		// Camera controls
		camera_.set_boost(keyboard_[GLFW_KEY_LEFT_CONTROL]);

		if (keyboard_[GLFW_KEY_W])
			camera_.move(Camera::Move::Forward, dt);

		if (keyboard_[GLFW_KEY_A])
			camera_.move(Camera::Move::Left, dt);

		if (keyboard_[GLFW_KEY_S])
			camera_.move(Camera::Move::Backward, dt);

		if (keyboard_[GLFW_KEY_D])
			camera_.move(Camera::Move::Right, dt);

		if (keyboard_[GLFW_KEY_SPACE])
			camera_.move(Camera::Move::Up, dt);

		if (keyboard_[GLFW_KEY_LEFT_SHIFT])
			camera_.move(Camera::Move::Down, dt);

		if (keyboard_[GLFW_KEY_Z])
			camera_.zoom_reset();
	}

	bool App::key_pressed(const int& key)
	{
		return keyboard_[key];
	}

	void App::show_fps(const float& newTime)
	{
		// Keep track of current time and frame count
		static float currentTime(0.0f);
		static int frameCount;
		const float deltaTime(newTime - currentTime);

		// Limit FPS refresh rate to 4 times per second
		if (deltaTime > 0.25f) {

			// Update current time, calculate FPS and frame time
			currentTime = newTime;
			const float fps(frameCount / deltaTime);
			const float msPerFrame(1000.0f / fps);

			// Keep track of minimum FPS
			static float minfps(FLT_MAX);
			if (fps > 1.0f && fps < minfps)
				minfps = fps;

			// Keep track of maximum FPS
			static float maxfps(FLT_MIN);
			if (fps > maxfps)
				maxfps = fps;

			// Keep track of average fps
			static float avgfps(60);
			avgfps = (avgfps + fps) / 2;

			// Display information in window title
			std::ostringstream output;
			output.precision(3);
			output << std::fixed << "OpenGL - FPS: " << fps << " - Time: " << msPerFrame << "ms" << " - MAX: " << maxfps << " - MIN: " << minfps << " - AVG: " << avgfps << std::endl;
			glfwSetWindowTitle(window_, output.str().c_str());

			frameCount = 0;
		}

		++frameCount;
	}

	///////////////////////////////////////////////////////
	// RENDER

	void App::clear()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void App::draw(const Mesh& mesh)
	{
		const Shader shader(mesh.get_shader());

		// Return immediately if shader is invalid
		if (!shader.use())
			return;

		// Bind matrices
		shader.bind("translate", mesh.get_translate());
		shader.bind("rotate", mesh.get_rotate());
		shader.bind("scale", mesh.get_scale());

		shader.bind("view", camera_.get_view());
		shader.bind("projection", camera_.get_projection());

		glBindVertexArray(mesh.get_vao());
		glDrawArrays(GL_TRIANGLES, 0, mesh.get_size());

		glBindVertexArray(0);
	}

	void App::display()
	{
		glBindVertexArray(0);
		glfwSwapBuffers(window_);
	}

	///////////////////////////////////////////////////////
	// CLEANUP

	bool App::should_close()
	{
		return glfwWindowShouldClose(window_);
	}

	void App::close()
	{
		glfwTerminate();
	}

	///////////////////////////////////////////////////////
	// CALLBACK FUNCTIONS

	void App::key_callback(GLFWwindow* window, const int key, int scancode, const int action, int mode)
	{
		// Close window
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		// Set key state
		if (key >= 0 && key <= 1024) {
			if (action == GLFW_PRESS)
				keyboard_[key] = true;

			else if (action == GLFW_RELEASE)
				keyboard_[key] = false;
		}
	}

	void App::mouse_callback(GLFWwindow*, const double x_position, const double y_position)
	{
		// Callback position
		const glm::vec2 position(x_position, y_position);

		// Set first mouse poll
		if (first_mouse_) {
			mouse_pos_ = position;
			first_mouse_ = false;
		}

		// Offsets
		const glm::vec2 offset(position.x - mouse_pos_.x, mouse_pos_.y - position.y);

		// Update mouse position
		mouse_pos_ = position;

		// Process mouse movement
		camera_.aim(offset);
	}

	void App::scroll_callback(GLFWwindow*, double x_offset, const double y_offset)
	{
		camera_.zoom(float(y_offset));
	}

	///////////////////////////////////////////////////////////
	// Window handle

	GLFWwindow* App::window_ = nullptr;

	///////////////////////////////////////////////////////////
	// CAMERA

	Camera App::camera_ = Camera();

	///////////////////////////////////////////////////////////
	// DIMENSIONS

	int App::screen_height_ = 0;
	int App::screen_width_ = 0;

	///////////////////////////////////////////////////////////
	// Input

	bool App::first_mouse_ = true;
	bool App::keyboard_[1024] = { false };
	glm::vec2 App::mouse_pos_ = glm::vec2(width, height) / 2.0f;

}
