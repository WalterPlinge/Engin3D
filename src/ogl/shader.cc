#include "shader.hh"

/////////////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <fstream>
#include <iostream>
#include <memory>

// GLM
#include <glm/gtc/type_ptr.hpp>

/////////////////////////////////////////////////////////////////////
// RENDER NAMESPACE

namespace ogl {

	/////////////////////////////////////////////////////////////////
	// CONSTRUCTORS

	Shader::Shader() = default;

	///////////////////////////////////////////////////////
	// ADD SHADER

	void Shader::add(const std::string& filename, const GLenum& type)
	{
		const std::string code(file_content(filename));

		const GLuint shader(compile(code, type));

		if (!shader)
			throw std::runtime_error("ERROR -> SHADER -> ADD -> " + filename + " FAIL");

		shaders_.push_back(shader);
	}

	void Shader::add(const std::vector<std::string>& filenames, const GLenum& type)
	{
		if (filenames.empty())
			throw std::runtime_error("ERROR -> SHADER -> ADD -> FILENAMES -> EMPTY");

		std::string code;
		for (const std::string& f : filenames)
			code += file_content(f);

		const GLuint shader(compile(code, type));

		if (!shader)
			throw std::runtime_error("ERROR -> SHADER -> ADD -> FILENAMES -> FAIL");

		shaders_.push_back(shader);
	}

	///////////////////////////////////////////////////////
	// BUILD PROGRAM

	bool Shader::build() noexcept(false)
	{
		program_ = glCreateProgram();

		for (const GLuint& s : shaders_)
			glAttachShader(program_, s);

		glLinkProgram(program_);

		GLint success;
		glGetProgramiv(program_, GL_LINK_STATUS, &success);

		// Return if program successfully linked
		if (success)			
			return program_;

		// Program info log
		GLint log_length;
		glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &log_length);

		std::unique_ptr<char> log(new char[log_length]);
		glGetProgramInfoLog(program_, log_length, &log_length, log.get());

		glDeleteProgram(program_);

		const std::string error("ERROR -> SHADER -> LINK -> FAIL: ");
		throw std::runtime_error(error + log.get());
	}

	/////////////////////////////////////////////////////////////////
	// USE PROGRAM

	bool Shader::use() const
	{
		// Program 0 is invalid and thus returns false
		glUseProgram(program_);
		return program_;
	}

	///////////////////////////////////////////////////////
	// BINDING

	void Shader::bind(const std::string& name, const glm::mat4& value, const GLsizei& count, const GLboolean& transpose) const
	{
		glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), count, transpose, glm::value_ptr(value));
	}

	///////////////////////////////////////////////////////
	// FILE OPERATIONS

	std::string Shader::file_content(const std::string& path) noexcept(false)
	{
		std::ifstream file(path);

		if (file.fail())
			throw std::runtime_error("ERROR -> SHADER -> FILE -> FAIL: " + path);

		return std::string(std::istreambuf_iterator<char>(file), {});
	}

	///////////////////////////////////////////////////////
	// COMPILATION

	GLuint Shader::compile(const std::string& code, const GLenum& type) noexcept(false)
	{
		// Throw if empty string
		if (code.empty())
			return 0;

		const GLchar* c_code(code.c_str());

		const GLuint shader(glCreateShader(type));
		glShaderSource(shader, 1, &c_code, nullptr);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		// Return if shader successfully compiled
		if (success)
			return shader;

		// Shader compile log
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

		std::unique_ptr<char> log(new char[log_length]);
		glGetShaderInfoLog(shader, log_length, &log_length, log.get());

		glDeleteShader(shader);

		// Shader type
		std::string shader_type;
		if (type == GL_VERTEX_SHADER)
			shader_type = "VERTEX";
		else if (type == GL_FRAGMENT_SHADER)
			shader_type = "FRAGMENT";
		else
			shader_type = "UNKNOWN TYPE";

		// Throw error
		const std::string error("ERROR -> SHADER -> COMPILE -> " + shader_type + " -> FAIL: ");
		throw std::runtime_error(error + log.get());
	}

	///////////////////////////////////////////////////////
	// LINKING

	GLuint Shader::link(const GLuint& vert_shader, const GLuint& frag_shader) noexcept(false)
	{
		const GLuint program(glCreateProgram());
		glAttachShader(program, vert_shader);
		glAttachShader(program, frag_shader);
		glLinkProgram(program);

		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);

		// Return if program successfully linked
		if (success)
			return program;

		// Program info log
		GLint log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

		std::unique_ptr<char> log(new char[log_length]);
		glGetProgramInfoLog(program, log_length, &log_length, log.get());

		glDeleteProgram(program);

		const std::string error("ERROR -> SHADER -> LINK -> FAIL: ");
		throw std::runtime_error(error + log.get());
	}

	///////////////////////////////////////////////////////
	// CLEANUP

	void Shader::clean()
	{
		for (GLuint shader : shaders_)
			glDeleteShader(shader);
		shaders_.clear();

		glDeleteProgram(program_);
		program_ = 0;
	}
}
