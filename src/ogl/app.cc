#include <e3d/ogl/app.hh>

#include <utility>
#include <vector>

#include <e3d/ogl/renderer.hh>



namespace ogl::app
{

// Functions
std::function<void()>      setup_function        = [](){};
std::function<void()>      input_function        = [](){};
std::function<void(float)> update_function       = [](float){};
std::function<void(float)> fixed_update_function = [](float){};
std::function<void()>      render_function       = [](){};
std::function<void()>      close_function        = [](){};



// Setup
auto static
setup(
	)
	-> void
{
	renderer::start();
	setup_function();
}



// Input
auto static
input(
	)
	-> void
{
	renderer::camera.boost = renderer::keyboard()[GLFW_KEY_LEFT_CONTROL];

	if (renderer::keyboard()[GLFW_KEY_Z])
		renderer::camera.zoom_reset();

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
	auto const static movements = std::vector<std::pair<Camera::Move, GLenum>>
	{
		{ Camera::Right,    GLFW_KEY_D },
		{ Camera::Left,     GLFW_KEY_A },
		{ Camera::Forward,  GLFW_KEY_W },
		{ Camera::Backward, GLFW_KEY_S },
		{ Camera::Up,       GLFW_KEY_SPACE },
		{ Camera::Down,     GLFW_KEY_LEFT_SHIFT }
	};

	for (auto const& m : movements)
		if (renderer::keyboard()[m.second])
			renderer::camera.move(m.first, delta_time);

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



// Close
auto static
close(
	)
	-> void
{
	close_function();
	renderer::shutdown();
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
	auto           current_time = renderer::clock::now();

	while (renderer::is_running())
	{
		auto       new_time   = renderer::clock::now();
		auto const delta_time = renderer::duration(new_time - current_time).count();
		current_time          = new_time;
		accumulator          += delta_time;

		renderer::update(new_time);

		input();

		update(delta_time);

		while (accumulator >= tick_rate)
		{
			accumulator -= tick_rate;
			time        += tick_rate;

			fixed_update(tick_rate);
		}

		render();
	}

	close();
}



} // namespace ogl::app
