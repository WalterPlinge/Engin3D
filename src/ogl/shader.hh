#ifndef OGL_SHADER_H
#define OGL_SHADER_H
#pragma once

///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <string>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLM
#include <glm/mat4x4.hpp>

///////////////////////////////////////////////////////////////
// RENDER NAMESPACE

namespace ogl {

	///////////////////////////////////////////////////////////
	// SHADER CLASS

	class Shader {
	public:

		///////////////////////////////////////////////////////
		// CONSTRUCTORS

		Shader();

		///////////////////////////////////////////////////////
		// ADD SHADER

		void add(const std::string& filename, const GLenum& type);
		void add(const std::vector<std::string>& filenames, const GLenum& type);

		///////////////////////////////////////////////////////
		// BUILD PROGRAM

		bool build() noexcept(false);

		///////////////////////////////////////////////////////
		// USE PROGRAM

		bool use() const;

		///////////////////////////////////////////////////////
		// BINDING

		void bind(const std::string& name, const glm::mat4& value, const GLsizei& count = 1, const GLboolean& transpose = GL_FALSE) const;

	private:

		///////////////////////////////////////////////////////
		// FILE OPERATIONS

		static std::string file_content(const std::string& path) noexcept(false);

		///////////////////////////////////////////////////////
		// COMPILATION

		static GLuint compile(const std::string& code, const GLenum& type) noexcept(false);

		///////////////////////////////////////////////////////
		// CLEANUP

		void clean();

		///////////////////////////////////////////////////////
		// ATTRIBUTES

		GLuint program_ = 0;
		std::vector<GLuint> shaders_;
	};
}

#endif