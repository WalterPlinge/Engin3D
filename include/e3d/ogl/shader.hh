#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/ext.hpp>

namespace ogl
{

class Shader
{
public:

	// Specify whether string is sourcecode or filename
	enum CodeType
	{
		Source,
		File
	};



	// Types
	// Shader code as array of source type and string to use
	using code_array = std::vector<std::pair<CodeType, std::string_view>>;

private:

	using uniform_cache = std::unordered_map<std::string, GLint>;



	// Details
	GLuint                program_ = 0;
	std::vector<GLuint>   shaders_;
	uniform_cache mutable uniforms_;

public:

	std::string name;



	// Constructors
	explicit
	Shader(
		std::string name = std::string()
		);

	Shader(
		Shader&&
		)
		= delete;

	Shader(
		Shader const&
		)
		= delete;

	auto
	operator=(
		Shader&&
		)
		-> Shader&
		= delete;

	auto
	operator=(
		Shader const&
		)
		-> Shader&
		= delete;

	~Shader(
		);



	// Queries
	auto
	is_valid(
		) const
		-> bool;

	auto
	is_active(
		) const
		-> bool;



	// Add shader code
	auto
	add(
		GLenum           shader_type,
		CodeType         code_type,
		std::string_view code
		)
		-> void;

	auto
	add(
		GLenum            shader_type,
		code_array const& code_array
		)
		-> void;



	// Management
	auto
	build(
		)
		-> void;

	auto
	use(
		) const
		-> void;

	auto
	clean(
		)
		-> void;

private:

	// Uniform caching
	auto
	uniform_location(
		std::string_view uniform
		) const
		-> GLint;



	// Program building
	auto
	compile(
		GLenum           type,
		std::string_view code
		) const
		-> std::optional<GLuint>;

	auto
	link(
		) const
		-> std::optional<GLuint>;

public:

	// Bind value to named uniform in program
	template<typename T>
	auto
	bind(
		std::string_view const uniform,
		T                const value
		) const
		-> void
	{
		auto const location = uniform_location(uniform);

		// Constexpr if allows branching to be removed at compile time
		if constexpr (std::is_same<T, GLint>::value)
			glUniform1i(location, value);
		else if constexpr (std::is_same<T, GLuint>::value)
			glUniform1ui(location, value);
		else if constexpr (std::is_same<T, GLfloat>::value)
			glUniform1f(location, value);
		else if constexpr (std::is_same<T, GLdouble>::value)
			glUniform1d(location, value);
		else
			std::cerr << "ERROR: Cannot bind uniform " <<
				uniform << " to value of type " <<
				typeid(T).name() << " in shader " <<
				name << std::endl;
	}

	// Bind value to named uniform in program
	template<typename T>
	auto
	bind(
		std::string_view const uniform,
		T const*         const value,
		GLsizei          const count
		) const
		-> void
	{
		auto const location = uniform_location(uniform);

		// Constexpr if allows branching to be removed at compile time
		if constexpr (std::is_same<T, GLint>::value)
			glUniform1iv(location, count, value);
		else if constexpr (std::is_same<T, GLuint>::value)
			glUniform1uiv(location, count, value);
		else if constexpr (std::is_same<T, GLfloat>::value)
			glUniform1fv(location, count, value);
		else if constexpr (std::is_same<T, GLdouble>::value)
			glUniform1dv(location, count, value);
		else
			std::cerr << "ERROR: Cannot bind uniform " <<
				uniform << " to value of type " <<
				typeid(T).name() << " in shader " <<
				name << std::endl;
	}

	// Bind vector value to named uniform in program
	template<glm::length_t L, typename T>
	auto
	bind(
		std::string_view const uniform,
		glm::vec<L, T>   const value,
		GLsizei          const count = 1
		) const
		-> void
	{
		auto const location = uniform_location(uniform);

		// Constexpr if allows branching to be removed at compile time
		if constexpr (std::is_same<T, GLint>::value)
		{
			if constexpr (L == 1)
				glUniform1iv(location, count, glm::value_ptr(value));
			else if constexpr (L == 2)
				glUniform2iv(location, count, glm::value_ptr(value));
			else if constexpr (L == 3)
				glUniform3iv(location, count, glm::value_ptr(value));
			else if constexpr (L == 4)
				glUniform4iv(location, count, glm::value_ptr(value));
			else
				std::cerr << "ERROR: Cannot bind uniform " <<
					uniform << " to vector of length " <<
					L << " in shader " <<
					name << std::endl;
		}
		else if constexpr (std::is_same<T, GLuint>::value)
		{
			if constexpr (L == 1)
				glUniform1uiv(location, count, glm::value_ptr(value));
			else if constexpr (L == 2)
				glUniform2uiv(location, count, glm::value_ptr(value));
			else if constexpr (L == 3)
				glUniform3uiv(location, count, glm::value_ptr(value));
			else if constexpr (L == 4)
				glUniform4uiv(location, count, glm::value_ptr(value));
			else
				std::cerr << "ERROR: Cannot bind uniform " <<
					uniform << " to vector of length " <<
					L << " in shader " <<
					name << std::endl;
		}
		else if constexpr (std::is_same<T, GLfloat>::value)
		{
			if constexpr (L == 1)
				glUniform1fv(location, count, glm::value_ptr(value));
			else if constexpr (L == 2)
				glUniform2fv(location, count, glm::value_ptr(value));
			else if constexpr (L == 3)
				glUniform3fv(location, count, glm::value_ptr(value));
			else if constexpr (L == 4)
				glUniform4fv(location, count, glm::value_ptr(value));
			else
				std::cerr << "ERROR: Cannot bind uniform " <<
					uniform << " to vector of length " <<
					L << " in shader " <<
					name << std::endl;
		}
		else if constexpr (std::is_same<T, GLdouble>::value)
		{
			if constexpr (L == 1)
				glUniform1dv(location, count, glm::value_ptr(value));
			else if constexpr (L == 2)
				glUniform2dv(location, count, glm::value_ptr(value));
			else if constexpr (L == 3)
				glUniform3dv(location, count, glm::value_ptr(value));
			else if constexpr (L == 4)
				glUniform4dv(location, count, glm::value_ptr(value));
			else
				std::cerr << "ERROR: Cannot bind uniform " <<
					uniform << " to vector of length " <<
					L << " in shader " <<
					name << std::endl;
		}
		else
			std::cerr << "ERROR: Cannot bind uniform " <<
				uniform << " to value of type " <<
				typeid(T).name() << " in shader " <<
				name << std::endl;
	}

	// Bind matrix value to named uniform in program
	template<glm::length_t C, glm::length_t R, typename T>
	auto
	bind(
		std::string_view  const uniform,
		glm::mat<C, R, T> const value,
		GLsizei           const count     = 1,
		GLboolean         const transpose = GL_FALSE
		) const
		-> void
	{
		auto const location = uniform_location(uniform);

		// Constexpr allows branching to be removed at compile time
		if constexpr (std::is_same<T, GLfloat>::value)
		{
			if constexpr (C == 2 && R == 2)
				glUniformMatrix2fv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 2 && R == 3)
				glUniformMatrix2x3fv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 2 && R == 4)
				glUniformMatrix2x4fv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 3 && R == 2)
				glUniformMatrix3x2fv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 3 && R == 3)
				glUniformMatrix3fv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 3 && R == 4)
				glUniformMatrix3x4fv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 4 && R == 2)
				glUniformMatrix4x2fv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 4 && R == 3)
				glUniformMatrix4x3fv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 4 && R == 4)
				glUniformMatrix4fv(location, count, transpose, glm::value_ptr(value));
			else
				std::cerr << "ERROR: Cannot bind uniform " <<
					uniform << " to matrix size " <<
					C << "x" << R << " in shader " <<
					name << std::endl;
		}
		else if constexpr (std::is_same<T, GLdouble>::value)
		{
			if constexpr (C == 2 && R == 2)
				glUniformMatrix2dv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 2 && R == 3)
				glUniformMatrix2x3dv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 2 && R == 4)
				glUniformMatrix2x4dv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 3 && R == 2)
				glUniformMatrix3x2dv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 3 && R == 3)
				glUniformMatrix3dv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 3 && R == 4)
				glUniformMatrix3x4dv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 4 && R == 2)
				glUniformMatrix4x2dv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 4 && R == 3)
				glUniformMatrix4x3dv(location, count, transpose, glm::value_ptr(value));
			else if constexpr (C == 4 && R == 4)
				glUniformMatrix4dv(location, count, transpose, glm::value_ptr(value));
			else
				std::cerr << "ERROR: Cannot bind uniform " <<
				uniform << " to matrix size " <<
				C << "x" << R << " in shader " <<
				name << std::endl;
		}
		else
			std::cerr << "ERROR: Cannot bind uniform " <<
				uniform << " to value of type " <<
				typeid(T).name() << " in shader " <<
				name << std::endl;
	}
};

} // namespace ogl
