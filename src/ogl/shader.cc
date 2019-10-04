#include "shader.hh"

// STD
#include <fstream>
#include <memory>
#include <optional>
#include <string>

// GLM
#include <glm/gtc/type_ptr.hpp>
#include <utility>

// Namespace
using namespace std::string_literals;
using namespace ogl;



// Return contents of file
static auto file_content(
	const std::string& path
	) -> std::optional<std::string>
{
	std::ifstream file(path);

	if(file.good())
		return std::string(std::istreambuf_iterator<char>(file), {});

	std::cerr << "ERROR: Cannot open shader file " <<
		path << std::endl;
	return std::nullopt;
}



// Class management
Shader::Shader(
	std::string name
	) : name_(std::move(name))
{}

Shader::~Shader(
	)
{ clean(); }



// Name
auto Shader::name(
	) const -> std::string
{ return name_; }

auto Shader::name(
	const std::string& name
	) -> void
{ name_ = name; }



// Add shader type, providing code as string or filename
auto Shader::add(
	const GLenum&      type,
	const std::string& code,
	const bool&        is_file
	) -> void
{
	const auto content = is_file
		? file_content(code).value_or(""s)
		: code;

	const auto shader = compile(type, content);

	if (shader)
		shaders_.push_back(shader);
}

// Add shader type, providing code as string or filename
auto Shader::add(
	const GLenum&     type,
	const code_array& is_file_code
	) -> void
{
	auto content = std::string{};
	for (const auto& c : is_file_code)
	{
		if (c.first)
			content += file_content(c.second).value_or(""s);
		else
			content += c.second;

		content += '\n';
	}

	const auto shader = compile(type, content);

	if (shader)
		shaders_.push_back(shader);
}



// Build shader program
auto Shader::build(
	) -> bool
{
	program_ = glCreateProgram();
	for (const auto& s : shaders_)
		glAttachShader(program_, s);
	glLinkProgram(program_);


	// Return if program successfully linked
	auto success = GLint{};
	glGetProgramiv(program_, GL_LINK_STATUS, &success);
	if (success)
		return program_;

	// Program info log
	auto log_length = GLint{};
	glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &log_length);

	const auto log = std::make_unique<char[]>(log_length);
	glGetProgramInfoLog(program_, log_length, &log_length, log.get());

	std::cerr << "ERROR build failed for shader " <<
		name_ << ". Log: " <<
		log.get() << std::endl;

	// Clean up
	glDeleteProgram(program_);
	return program_ = 0;
}



// Use shader program
auto Shader::use(
	) const -> bool
{
	glUseProgram(program_);
	auto program = GLint{};
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	return program == program_;
}



// Clean up
auto Shader::clean(
	) -> void
{
	glUseProgram(0);

	for (auto shader : shaders_)
		glDeleteShader(shader);
	shaders_.clear();

	glDeleteProgram(program_);
	program_ = 0;
}



// Compile shader
auto Shader::compile(
	const GLenum&      type,
	const std::string& code
	) const -> GLuint
{
	// Shader type
	auto shader_type = std::string{};
	switch (type)
	{
		case GL_VERTEX_SHADER:   shader_type = "VERTEX";   break;
		case GL_FRAGMENT_SHADER: shader_type = "FRAGMENT"; break;
		default:                 shader_type = "UNKNOWN";  break;
	}

	// Create shader ID and pass code pointer for compilation
	const auto shader_code = code.c_str();
	const auto shader      = glCreateShader(type);
	glShaderSource(shader, 1, &shader_code, nullptr);
	glCompileShader(shader);

	auto success = GLint{};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	// Return if shader successfully compiled
	if (success)
		return shader;

	// Get compilation log
	auto log_length = GLint{};
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

	const auto log = std::make_unique<char[]>(log_length);
	glGetShaderInfoLog(shader, log_length, &log_length, log.get());

	std::cerr << "ERROR: " <<
		name_ << " " <<
		shader_type << " shader failed to compile. Log: " <<
		log.get() << std::endl;

	// Clean up
	glDeleteShader(shader);
	return 0;
}
