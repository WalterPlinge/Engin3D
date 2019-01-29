#ifndef PHYSICS_BODY_H
#define PHYSICS_BODY_H
#pragma once

///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <memory>
#include <vector>

// GLM
#include <glm/vec3.hpp>

// Project
#include <ogl/mesh.hh>
#include <ogl/shader.hh>
#include <phy/force.hh>

///////////////////////////////////////////////////////////////
// PHYSICS NAMESPACE

namespace phy {

	class Body {
	public:

		///////////////////////////////////////////////////////
		// CONSTRUCTOR

		explicit Body(ogl::Mesh mesh = ogl::Mesh());

		///////////////////////////////////////////////////////
		// GETTERS

		// Movement
		glm::vec3 get_position() const;
		glm::vec3 get_velocity() const;
		glm::vec3 get_acceleration() const;

		// Properties
		float get_area() const;
		float get_drag() const;
		float get_elasticity() const;
		float get_mass() const;

		// Mesh
		ogl::Mesh& get_mesh();

		///////////////////////////////////////////////////////
		// SETTERS

		// Movement
		void set_position(const glm::vec3& position);
		void set_velocity(const glm::vec3& velocity);
		void set_acceleration(const glm::vec3& acceleration);

		// Properties
		void set_area(const float& area);
		void set_drag(const float& drag);
		void set_elasticity(const float& elasticity);
		void set_mass(const float& mass);

		// Mesh
		void set_mesh(const ogl::Mesh& mesh);

		///////////////////////////////////////////////////////
		// TRANSFORMS

		void rotate(const float& angle, const glm::vec3& vector);
		void scale(const glm::vec3& vector);
		void translate(const glm::vec3& vector);
		void reset_transforms();

		///////////////////////////////////////////////////////
		// FORCES

		void add_force(const std::shared_ptr<Force>& force);
		glm::vec3 calculate_forces();

	private:

		///////////////////////////////////////////////////////
		// ATTRIBUTES

		glm::vec3 position_ = glm::vec3(0.0f);
		glm::vec3 velocity_ = glm::vec3(0.0f);
		glm::vec3 acceleration_ = glm::vec3(0.0f);

		float area_ = 0.0f;
		float drag_ = 0.0f;
		float elasticity_ = 0.0f;
		float mass_ = 0.0f;

		ogl::Mesh mesh_ = ogl::Mesh();

		std::vector<std::shared_ptr<Force>> forces_;
	};
}

#endif
