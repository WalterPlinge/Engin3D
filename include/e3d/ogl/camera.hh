#pragma once

#include <glm/glm.hpp>

namespace ogl
{

class Camera
{
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

private:

	// World vectors
	glm::vec3 const static world_right_;
	glm::vec3 const static world_front_;
	glm::vec3 const static world_up_;

public:

	glm::vec3 position = glm::vec3(0.0F);

	float sensitivity = 0.25F;
	float speed       = 4.0F;
	bool  boost       = false;

	// Attributes
	float fov    = 45.0F;
	float near   = 0.01F;
	float far    = 100.0F;

private:

	float aspect_ = 1.0F;

	// Angles
	float pitch_ = 0.0F;
	float yaw_   = 0.0F;

	// Local vectors
	glm::vec3 right_ = glm::vec3(1.0F, 0.0F, 0.0F);
	glm::vec3 front_ = glm::vec3(0.0F, 1.0F, 0.0F);
	glm::vec3 up_    = glm::vec3(0.0F, 0.0F, 1.0F);

public:

	// Constructor
	explicit
	Camera(
		);



	// Attributes
	auto
	aspect(
		float width,
		float height
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

	// N.B. potentially broken
	auto
	look_at(
		glm::vec3 point
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
};

} // namespace ogl
