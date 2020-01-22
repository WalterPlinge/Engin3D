#include <e3d/ogl/renderer.hh>

#include <sstream>



namespace ogl::renderer
{

// Details
std::string    title  = "Engin3D";
std::uintmax_t width  = 1280U;
std::uintmax_t height = 720U;

// Camera
Camera camera;

// Window handle
GLFWwindow static* window_ = nullptr;

// Dimensions
auto static screen_width_  = 0;
auto static screen_height_ = 0;

// Input
auto static first_mouse_ = true;
auto static mouse_pos_   = glm::vec2();
auto static keyboard_    = keyboard_t{ false };



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
	time_point const new_time
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

auto
is_running(
	)
	-> bool
{
	return !glfwWindowShouldClose(window_);
}



// Initialise
auto
start(
	)
	-> void
{
	// Set close function to run at exit
	std::atexit(shutdown);



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
		shutdown();
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
		shutdown();
		throw std::runtime_error("ERROR: GLEW failed to initialise");
	}

	// Define the viewport dimensions
	glViewport(0, 0, screen_width_, screen_height_);

	// Set up some other opengl options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.aspect(screen_width_, screen_height_);

	std::cout << "App initialised" << std::endl;
}

auto
update(
	time_point const new_time
	)
	-> void
{
	glfwPollEvents();
	show_fps(new_time);
}



// Render
auto
bind(
	Texture const& texture,
	GLuint         index
	)
	-> void
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(texture.type(), texture.id());
}

auto
target(
	std::optional<Framebuffer const*> const framebuffer
	)
	-> void
{
	glBindFramebuffer(
		GL_FRAMEBUFFER,
		framebuffer.has_value()
			? framebuffer.value()->id()
			: 0);
}

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
shutdown(
	)
	-> void
{
	glfwTerminate();
	window_ = nullptr;
}



} // namespace ogl::renderer
