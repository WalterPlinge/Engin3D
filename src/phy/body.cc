#include <phy/body.hh>

///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <utility>

// Project
#include <phy/force.hh>

///////////////////////////////////////////////////////////////
// PHYSICS NAMESPACE

namespace phy {

	///////////////////////////////////////////////////////
	// CONSTRUCTOR

	Body::Body(ogl::Mesh mesh) : mesh_(std::move(mesh)) {}

	///////////////////////////////////////////////////////
	// GETTERS

	// Movement
	glm::vec3 Body::get_position() const
	{
		return position_;
	}

	glm::vec3 Body::get_velocity() const
	{
		return velocity_;
	}

	glm::vec3 Body::get_acceleration() const
	{
		return acceleration_;
	}

	// Properties
	float Body::get_area() const
	{
		return area_;
	}

	float Body::get_drag() const
	{
		return drag_;
	}

	float Body::get_elasticity() const
	{
		return elasticity_;
	}

	float Body::get_mass() const
	{
		return mass_;
	}

	// Mesh
	ogl::Mesh& Body::get_mesh()
	{
		return mesh_;
	}

	///////////////////////////////////////////////////////
	// SETTERS

	// Movement
	void Body::set_position(const glm::vec3& position)
	{
		position_ = position;
		mesh_.set_position(position);
	}

	void Body::set_velocity(const glm::vec3& velocity)
	{
		velocity_ = velocity;
	}

	void Body::set_acceleration(const glm::vec3& acceleration)
	{
		acceleration_ = acceleration;
	}

	// Properties
	void Body::set_area(const float& area)
	{
		area_ = area;
	}

	void Body::set_drag(const float& drag)
	{
		drag_ = drag;
	}

	void Body::set_elasticity(const float& elasticity)
	{
		elasticity_ = elasticity;
	}

	void Body::set_mass(const float& mass)
	{
		mass_ = mass;
	}

	// Mesh
	void Body::set_mesh(const ogl::Mesh& mesh)
	{
		mesh_ = mesh;
	}

	///////////////////////////////////////////////////////
	// TRANSFORMS

	void Body::rotate(const float& angle, const glm::vec3& vector)
	{
		mesh_.rotate(angle, vector);
	}

	void Body::scale(const glm::vec3& vector)
	{
		mesh_.scale(vector);
	}
	
	void Body::translate(const glm::vec3& vector)
	{
		position_ += vector;
		mesh_.translate(vector);
	}
	
	void Body::reset_transforms()
	{
		mesh_.reset_transforms();
	}

	///////////////////////////////////////////////////////
	// FORCES

	void Body::add_force(const std::shared_ptr<Force>& force)
	{
		forces_.push_back(force);
	}
	
	glm::vec3 Body::calculate_forces()
	{
		glm::vec3 force(0.0f);

		for (std::shared_ptr<Force>& f : forces_)
			force += f->apply(*this);

		return force;
	}
}
