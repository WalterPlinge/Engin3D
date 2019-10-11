#include "app.hh"

#include <ctime>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>



namespace ogl::app
{

// Camera
auto camera = Camera();

// Window handle
GLFWwindow static* window_ = nullptr;

// Dimensions
auto static constexpr width_         = 800u;
auto static constexpr height_        = 600u;
auto static           screen_width_  = 0;
auto static           screen_height_ = 0;

// Input
auto static first_mouse_ = true;
auto static mouse_pos_   = glm::vec2();
auto static keyboard_    = app::keyboard_t{ false };



// Callback functions
auto static
key_callback(
	GLFWwindow* const window,
	int         const key,
	int         const scancode,
	int         const action,
	int         const mods
	)
	-> void
{
	// Set key state
	if (key != GLFW_KEY_UNKNOWN)
	{
		if (action == GLFW_PRESS)
			keyboard_[std::size_t(key)] = true;
		else if (action == GLFW_RELEASE)
			keyboard_[std::size_t(key)] = false;
	}

	// Close window
	if (keyboard_[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
}

auto static
scroll_callback(
	GLFWwindow* const window,
	double      const x_offset,
	double      const y_offset
	)
	-> void
{
	camera.zoom(float(y_offset));
}

auto static
cursor_pos_callback(
	GLFWwindow* const window,
	double      const x_position,
	double      const y_position
	)
	-> void
{
	auto const pos = glm::vec2(x_position, y_position);

	// First mouse poll
	if (first_mouse_)
	{
		mouse_pos_   = pos;
		first_mouse_ = false;
	}

	// Offsets
	auto const offset = glm::vec2(pos.x - mouse_pos_.x, mouse_pos_.y - pos.y);

	// Update mouse position
	mouse_pos_ = pos;

	// Update camera
	camera.aim(offset);
}

auto static
window_size_callback(
	GLFWwindow* const window,
	int         const width,
	int         const height
	)
	-> void
{
	glfwGetFramebufferSize(window, &screen_width_, &screen_height_);
	glViewport(0, 0, screen_width_, screen_height_);
	camera.aspect(screen_width_, screen_height_);
}



// Initialise
auto
initialise_renderer(
	)
	-> void
{
	// Set close function to run at exit
	std::atexit(close);



	// Initialise GLFW
	if (!glfwInit())
		throw std::runtime_error("ERROR: GLFW failed to initialise");

	// GLFW settings (v3.3, core profile, not resizable, forward compatability)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Window error handling
	window_ = glfwCreateWindow(width_, height_, "Engin3D", nullptr, nullptr);

	if (!window_)
	{
		close();
		throw std::runtime_error("ERROR: GLFW failed to create window");
	}

	// Set context and framebuffer size
	glfwMakeContextCurrent(window_);
	glfwGetFramebufferSize(window_, &screen_width_, &screen_height_);

	// Set callback functions
	glfwSetKeyCallback(window_, key_callback);
	glfwSetScrollCallback(window_, scroll_callback);
	glfwSetCursorPosCallback(window_, cursor_pos_callback);
	glfwSetWindowSizeCallback(window_, window_size_callback);

	// Remove mouse cursor
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



	// Modern GLEW approach
	glewExperimental = GL_TRUE;

	// Initialise GLEW to setup the opengl function pointers
	if (glewInit() != GLEW_OK)
	{
		close();
		throw std::runtime_error("ERROR: GLEW failed to initialise");
	}

	// Define the viewport dimensions
	glViewport(0, 0, screen_width_, screen_height_);

	// Set up some other opengl options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.aspect(screen_width_, screen_height_);

	std::cout << "App initialised" << std::endl;
}



// Update
auto
update(
	float delta_time
	) -> void
{
	glfwPollEvents();

	camera.boost = keyboard_[GLFW_KEY_LEFT_CONTROL];

	if (keyboard_[GLFW_KEY_W])
		camera.move(Camera::Move::Forward, delta_time);

	if (keyboard_[GLFW_KEY_A])
		camera.move(Camera::Move::Left, delta_time);

	if (keyboard_[GLFW_KEY_S])
		camera.move(Camera::Move::Backward, delta_time);

	if (keyboard_[GLFW_KEY_D])
		camera.move(Camera::Move::Right, delta_time);

	if (keyboard_[GLFW_KEY_SPACE])
		camera.move(Camera::Move::Up, delta_time);

	if (keyboard_[GLFW_KEY_LEFT_SHIFT])
		camera.move(Camera::Move::Down, delta_time);

	if (keyboard_[GLFW_KEY_Z])
		camera.zoom_reset();
}

auto
show_fps(
	float new_time
	)
	-> void
{
	// Keep track of current time and frame count
	auto static current_time = 0.0F;
	auto static frame_count  = int{};
	auto const  delta_time   = new_time - current_time;

	// Limit FPS refresh rate to 4 times per second
	if (delta_time > 0.25F)
	{
		// Update current time, calculate FPS and frame time
		current_time   = new_time;
		auto const fps = static_cast<float>
			(frame_count) /
			delta_time;
		auto const ms_per_frame = 1000.0F / fps;

		// Display information in window title
		std::ostringstream output;
		output.precision(3);
		output << std::fixed << "ShaderToy"
			" - FPS: " << fps <<
			" - Frame: " << ms_per_frame << "ms" << std::endl;
		glfwSetWindowTitle(window_, output.str().c_str());

		frame_count = 0;
	}

	++frame_count;
}



// Render
auto
clear(
	)
	-> void
{
	glClearColor(1.0F, 0.0F, 0.0F, 1.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto
draw(
	Mesh const& mesh
	)
	-> void
{
	auto const& shader = mesh.shader;

	if (!shader.use())
		return;

	shader.bind("translate", mesh.translate());
	shader.bind("rotate", mesh.rotate());
	shader.bind("scale", mesh.scale());

	shader.bind("view", camera.view());
	shader.bind("projection", camera.projection());

	glBindVertexArray(mesh.vao());
	glDrawArrays(GL_TRIANGLES, 0, GLsizei(mesh.size()));

	glBindVertexArray(0);
}

auto
display(
	)
	-> void
{
	glBindVertexArray(0);
	glfwSwapBuffers(window_);
}




// Shutdown
auto
should_close(
	)
	-> bool
{
	return glfwWindowShouldClose(window_);
}

auto
close(
	)
	-> void
{
	glfwTerminate();
	window_ = nullptr;
}



// Utilities
auto
resolution(
	)
	-> glm::vec2
{
	return glm::vec2(screen_width_, screen_height_);
}

auto
keyboard(
	)
	-> const keyboard_t&
{
	return keyboard_;
}

auto
mouse_pos(
	)
	-> glm::vec2
{
	return mouse_pos_;
}

} // namespace ogl::app
