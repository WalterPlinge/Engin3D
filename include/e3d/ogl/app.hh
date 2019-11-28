#pragma once

#include <functional>

#include <e3d/ogl/renderer.hh>

namespace ogl::app
{

// Functions
std::function<void()>      extern setup_function;
std::function<void()>      extern input_function;
std::function<void(float)> extern update_function;
std::function<void(float)> extern fixed_update_function;
std::function<void()>      extern render_function;
std::function<void()>      extern close_function;

// Run application
auto
run(
	)
	-> void;

} // namespace ogl::app
