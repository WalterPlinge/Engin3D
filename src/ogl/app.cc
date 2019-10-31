#include <e3d/ogl/app.hh>

#include <chrono>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>



namespace ogl::app
{



using clock      = std::chrono::high_resolution_clock;
using              std::chrono::seconds;
using time_point = std::chrono::time_point<clock>;
using duration   = std::chrono::duration<float>;



// Details
std::string    title  = "Engin3D";
std::uintmax_t width  = 1280U;
std::uintmax_t height = 720U;

// Camera
Camera camera;

// Functions
std::function<void()>      setup_function        = [](){};
std::function<void()>      input_function        = [](){};
std::function<void(float)> update_function       = [](float){};
std::function<void(float)> fixed_update_function = [](float){};
std::function<void()>      render_function       = [](){};
std::function<void()>      close_function        = [](){};

// Window handle
GLFWwindow static* window_ = nullptr;

// Dimensions
auto static screen_width_  = 0;
auto static screen_height_ = 0;

// Input
auto static first_mouse_ = true;
auto static mouse_pos_   = glm::vec2();
auto static keyboard_    = keyboard_t{ false };



// Details
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

auto
clear_colour(
	glm::vec4 const colour
	)
	-> void
{
	glClearColor(colour.r, colour.g, colour.b, colour.a);
}



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
		{
			keyboard_[std::size_t(key)] = true;

			// Close window
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, GL_TRUE);
		}

		else if (action == GLFW_RELEASE)
			keyboard_[std::size_t(key)] = false;
	}
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

auto static
show_fps(
	time_point new_time
	)
	-> void
{
	// Keep track of current time and frame count
	auto static current_time = time_point();
	auto static frame_count  = int{};
	auto const  delta_time   = duration(new_time - current_time).count();

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
		output << std::fixed << title <<
			" - FPS: " << fps <<
			" - Frame: " << ms_per_frame << "ms" << std::endl;
		glfwSetWindowTitle(window_, output.str().c_str());

		frame_count = 0;
	}

	++frame_count;
}



// Initialise
auto static
setup(
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
	window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

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
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.aspect(screen_width_, screen_height_);

	// User defined function
	setup_function();

	std::cout << "App initialised" << std::endl;
}



// Input
auto static
input(
	)
	-> void
{
	glfwPollEvents();

	camera.boost = keyboard_[GLFW_KEY_LEFT_CONTROL];

	if (keyboard_[GLFW_KEY_Z])
		camera.zoom_reset();

	input_function();
}



// Update
auto static
update(
	float delta_time
	)
	-> void
{
	update_function(delta_time);
}



// Fixed update
auto static
fixed_update(
	float delta_time
	)
	-> void
{
	if (keyboard_[GLFW_KEY_W])
		camera.move(Camera::Forward, delta_time);

	if (keyboard_[GLFW_KEY_A])
		camera.move(Camera::Left, delta_time);

	if (keyboard_[GLFW_KEY_S])
		camera.move(Camera::Backward, delta_time);

	if (keyboard_[GLFW_KEY_D])
		camera.move(Camera::Right, delta_time);

	if (keyboard_[GLFW_KEY_SPACE])
		camera.move(Camera::Up, delta_time);

	if (keyboard_[GLFW_KEY_LEFT_SHIFT])
		camera.move(Camera::Down, delta_time);

	fixed_update_function(delta_time);
}



// Render
auto static
render(
	)
	-> void
{
	render_function();
}



// Run
auto
run(
	)
	-> void
{
	setup();

	auto           time         = 0.0F;
	auto           accumulator  = 0.0F;
	auto constexpr tick_rate    = 1.0F / 60.0F;
	auto           current_time = clock::now();

	while (!glfwWindowShouldClose(window_))
	{
		auto       new_time   = clock::now();
		auto const delta_time = duration(new_time - current_time).count();
		current_time          = new_time;
		accumulator          += delta_time;

		input();

		update(delta_time);
		show_fps(new_time);

		while (accumulator >= delta_time)
		{
			accumulator -= delta_time;
			time        += delta_time;

			fixed_update(tick_rate);
		}

		render();
	}

	close();
}



// Render
auto
clear(
	)
	-> void
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto
draw(
	Mesh const& mesh
	)
	-> void
{
	glBindVertexArray(mesh.vao());
	glDrawArrays(GL_TRIANGLES, 0, mesh.size());

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



// Close
auto
close(
	)
	-> void
{
	close_function();
	glfwTerminate();
	window_ = nullptr;
}

} // namespace ogl::app
