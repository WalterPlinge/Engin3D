#include <e3d/ogl/shader.hh>

#include <memory>
#include <sstream>
#include <utility>

#include <glm/ext.hpp>

#include <luna/files.hh>

using namespace std::string_literals;
using namespace std::string_view_literals;



namespace ogl
{

// Class management
Shader::
Shader(
	std::string name
	) :
	name(std::move(name))
{}

Shader::
~Shader(
	)
{
	clean();
}



// Queries
auto Shader::
id(
	) const
	-> GLuint
{
	return program_;
}

auto Shader::
uniforms(
	) const
	-> uniform_cache const&
{
	return uniforms_;
}

auto Shader::
is_valid(
	) const
	-> bool
{
	return program_ != 0;
}

auto Shader::
is_active(
	) const
	-> bool
{
	auto p = GLint{};
	glGetIntegerv(GL_CURRENT_PROGRAM, &p);
	return is_valid() && program_ == GLuint(p);
}



// Add shader code
auto Shader::
add(
	GLenum           const shader_type,
	CodeType         const code_type,
	std::string_view const code
	)
	-> void
{
	auto const shader = compile(shader_type, code_type == Source
		? code
		: luna::read_file(code).value_or(""s));

	if (shader.has_value())
		shaders_.emplace_back(shader.value());
}

auto Shader::
add(
	GLenum     const  type,
	code_array const& code_array
	)
	-> void
{
	auto content = std::ostringstream{};
	for (auto const& c : code_array)
	{
		if (c.first == Source)
			content << c.second;
		else
			content << luna::read_file(c.second).value_or(""s);
		content << "\n"sv;
	}

	add(type, Source, content.str());
}



// Management
auto Shader::
build(
	)
	-> void
{
	program_ = link().value_or(0);
}

auto Shader::
use(
	) const
	-> void
{
	glUseProgram(program_);
}

auto Shader::
clean(
	)
	-> void
{
	if (is_active())
		glUseProgram(0);

	for (auto const& s : shaders_)
		glDeleteShader(s);
	shaders_.clear();

	uniforms_.clear();

	glDeleteProgram(program_);
	program_ = 0;
}



// Uniform caching
auto Shader::
uniform_location(
	std::string_view const uniform
	) const
	-> GLint
{
	if (uniforms_.find(uniform.data()) != uniforms_.end())
		return uniforms_.at(uniform.data());

	return (uniforms_[uniform.data()] =
		glGetUniformLocation(
			program_,
			uniform.data()));
}



// Compile shader
auto Shader::
compile(
	GLenum           const type,
	std::string_view const code
	) const
	-> std::optional<GLuint>
{
	// Shader type
	auto shader_type = std::string_view();
	switch (type)
	{
		case GL_VERTEX_SHADER:   shader_type = "VERTEX"sv;   break;
		case GL_FRAGMENT_SHADER: shader_type = "FRAGMENT"sv; break;
		default:                 shader_type = "UNKNOWN"sv;  break;
	}

	// Create shader ID and pass code pointer for compilation
	auto const shader = glCreateShader(type);
	auto const shader_code = code.data();
	glShaderSource(shader, 1, &shader_code, nullptr);
	glCompileShader(shader);

	// Return shader ID if successful
	auto success = GLint{};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success)
		return shader;

	// Get compilation log
	auto log_length = GLint{};
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

	auto const log = std::make_unique<char[]>(std::size_t(log_length));
	glGetShaderInfoLog(shader, log_length, &log_length, log.get());

	std::cerr << "ERROR: " <<
		name << " " <<
		shader_type << " shader failed to compile. Log: " <<
		log.get() << std::endl;

	// Clean up
	glDeleteShader(shader);
	return std::nullopt;
}

auto Shader::
link(
	) const
	-> std::optional<GLuint>
{
	auto const program = glCreateProgram();
	for (auto const& s : shaders_)
		glAttachShader(program, s);
	glLinkProgram(program);

	// Returns program ID if successful
	auto success = GLint{};
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success)
		return program;

	// Get link log
	auto log_length = GLint{};
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

	auto const log = std::make_unique<char[]>(std::size_t(log_length));
	glGetProgramInfoLog(program, log_length, &log_length, log.get());

	std::cerr << "ERROR: build failed for shader " <<
		name << ". Log: " <<
		log.get() << std::endl;

	// Clean up
	glDeleteProgram(program);
	return std::nullopt;
}

} // namespace ogl
