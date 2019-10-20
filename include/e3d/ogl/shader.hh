#pragma once

#include <iostream>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

namespace ogl
{
	class Shader
	{
		// Program ID and list of shader IDs
		GLuint              program_ = 0;
		std::vector<GLuint> shaders_;

	public:

		std::string         name;

		// Shader code as array of bool (is string filename) and code
		using code_array = std::vector<std::pair<bool, std::string>>;

		// Class
		Shader(
			)
			= default;

		explicit
		Shader(
			std::string name
			);

		~Shader(
			);

		// Add shader type, providing code as string or filename
		auto
		add(
			GLenum             type,
			std::string const& code,
			bool               is_file = true
			)
			-> void;

		// Add shader type, proving code as strings or filenames
		auto
		add(
			GLenum            type,
			code_array const& is_file_code
			)
			-> void;

		// Build program
		auto
		build(
			)
			-> bool;

		// Use program
		auto
		use(
			) const
			-> bool;

		// Clean up
		auto
		clean(
			)
			-> void;

	private:

		// Compile shader
		auto
		compile(
			GLenum             type,
			std::string const& code
			) const
			-> GLuint;

	public:

		// Bind value to named uniform in program
		template<typename T>
		auto
		bind(
			std::string const& uniform,
			T                  value
			) const
			-> void
		{
			auto const location = glGetUniformLocation(program_, uniform.c_str());

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
			std::string const& uniform,
			T           const* value,
			GLsizei            count
			) const
			-> void
		{
			auto const location = glGetUniformLocation(program_, uniform.c_str());

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
			std::string    const& uniform,
			glm::vec<L, T> const& value,
			GLsizei               count = 1
			) const
			-> void
		{
			auto const location = glGetUniformLocation(program_, uniform.c_str());

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
			std::string       const& uniform,
			glm::mat<C, R, T> const& value,
			GLsizei                  count     = 1,
			GLboolean                transpose = GL_FALSE
			) const
			-> void
		{
			auto const location = glGetUniformLocation(program_, uniform.c_str());

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
}
