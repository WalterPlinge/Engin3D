#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace ogl
{

class Camera
{
private:

	// World vectors
	static const glm::vec3 world_right_;
	static const glm::vec3 world_front_;
	static const glm::vec3 world_up_;

	// Variables
	glm::vec3 position_ = glm::vec3(0.0F);

public:

	float sensitivity = 0.25F;
	float speed       = 10.0F;
	bool  boost       = false;

private:

	// Attributes
	float fov_    = 45.0F;
	float aspect_ = 1.0F;
	float near_   = 0.01F;
	float far_    = 100.0F;

	// Angles
	float pitch_ = 0.0F;
	float roll_  = 0.0F;
	float yaw_   = 0.0F;

	// Local vectors
	glm::vec3 right_ = glm::vec3(1.0F, 0.0F, 0.0F);
	glm::vec3 front_ = glm::vec3(0.0F, 1.0F, 0.0F);
	glm::vec3 up_    = glm::vec3(0.0F, 0.0F, 1.0F);

	// Matrices
	glm::mat4 projection_ = glm::mat4(1.0F);
	glm::mat4 view_       = glm::mat4(1.0F);

public:

	enum Move
	{
		Right,
		Left,
		Forward,
		Backward,
		Up,
		Down
	};



	// Constructor
	explicit
	Camera(
		);



	// Position
	auto
	position(
		) const
		-> glm::vec3;

	auto
	position(
		glm::vec3 p
		)
		-> void;



	// Attributes
	auto
	fov(
		float fov
		)
		-> void;

	auto
	aspect(
		float width,
		float height
		)
		-> void;

	auto
	near(
		float near
		)
		-> void;

	auto
	far(
		float far
		)
		-> void;



	// Angles
	auto
	pitch(
		float pitch
		)
		-> void;

	auto
	roll(
		float roll
		)
		-> void;

	auto
	yaw(
		float yaw
		)
		-> void;



	// Matrices
	auto
	projection(
		) const
		-> glm::mat4;

	auto
	view(
		) const
		-> glm::mat4;



	// Orientation
	auto
	aim(
		glm::vec2 look
		)
		-> void;

	auto
	aim(
		glm::vec3 look
		)
		-> void;

	auto
	look_at(
		glm::vec3 position
		)
		-> void;



	// Movement
	auto
	move(
		Move  direction,
		float delta_time
		)
		-> void;



	// Perspective
	auto
	zoom(
		float y_offset
		)
		-> void;

	auto
	zoom_reset(
		)
		-> void;

private:

	// Update
	auto
	update_vectors(
		)
		-> void;

	auto
	update_matrices(
		)
		-> void;
};

} // namespace ogl
