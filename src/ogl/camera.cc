#include "camera.hh"

///////////////////////////////////////////////////////////////
// HEADERS

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

///////////////////////////////////////////////////////////////
// RENDER NAMESPACE

namespace ogl {

	///////////////////////////////////////////////////////////
	// CONSTRUCTOR

	Camera::Camera()
	{
		// Keep camera updated
		update_vectors();
	}

	///////////////////////////////////////////////////////////
	// GETTERS

	// Variables
	glm::vec3 Camera::get_position() const
	{
		return position_;
	}

	// Matrices
	glm::mat4 Camera::get_projection() const
	{
		return projection_;
	}

	glm::mat4 Camera::get_view() const
	{
		return view_;
	}

	///////////////////////////////////////////////////////////
	// SETTERS

	// Variables
	void Camera::set_position(const glm::vec3& position)
	{
		position_ = position;
		update_matrices();
	}

	void Camera::set_sensitivity(const float& sensitivity)
	{
		sensitivity_ = sensitivity;
	}

	void Camera::set_speed(const float& speed)
	{
		speed_ = speed;
	}

	void Camera::set_boost(const bool& boost)
	{
		boost_ = boost;
	}

	// Attributes
	void Camera::set_fov(const float& fov)
	{
		fov_ = fov;
		update_matrices();
	}

	void Camera::set_aspect(const float& width, const float& height)
	{
		aspect_ = width / height;
		update_matrices();
	}

	void Camera::set_near(const float& near)
	{
		near_ = near;
		update_matrices();
	}

	void Camera::set_far(const float& far)
	{
		far_ = far;
		update_matrices();
	}



	// Angles
	void Camera::set_pitch(const float& pitch)
	{
		pitch_ = glm::radians(pitch);
		update_vectors();
	}

	void Camera::set_roll(const float& roll)
	{
		roll_ = glm::radians(roll);
		update_vectors();
	}

	void Camera::set_yaw(const float& yaw)
	{
		yaw_ = glm::radians(yaw);
		update_vectors();
	}

	///////////////////////////////////////////////////////
	// MOVEMENT FUNCTIONS

	// Orientation
	void Camera::aim(const glm::vec2& look)
	{
		// Update pitch and yaw by look vector scaled by sensitivity
		const glm::vec2 update(look * sensitivity_);
		pitch_ += update.x;
		yaw_ += update.y;

		// Keep pitch within bounds
		const float bounds(glm::half_pi<float>() - FLT_MIN);

		if (pitch_ > bounds)
			pitch_ = bounds;
		else if (pitch_ < -bounds)
			pitch_ = -bounds;
	}

	void Camera::aim(const glm::vec3& look)
	{
		// Update pitch, roll and yaw by look vector scaled by sensitivity
		const glm::vec3 update(look * sensitivity_);
		pitch_ += update.x;
		roll_ += update.y;
		yaw_ += update.z;

		// Keep pitch and roll within bounds
		const float bounds(glm::half_pi<float>() - FLT_MIN);

		if (pitch_ > bounds)
			pitch_ = bounds;
		else if (pitch_ < -bounds)
			pitch_ = -bounds;

		if (roll_ > bounds)
			roll_ = bounds;
		else if (roll_ < -bounds)
			roll_ = -bounds;

		update_vectors();
	}

	void Camera::look_at(const glm::vec3& position)
	{
		// New front vector
		const glm::vec3 front(glm::normalize(position - position_));

		// Get pitch and yaw from new front vector
		pitch_ = std::asin(-front.z);
		yaw_ = std::atan2(front.x, front.y);

		update_vectors();
	}

	// Position
	void Camera::move(const Move& direction, const float& delta_time)
	{
		// Calculate velocity in relation to delta_time and apply boost
		float velocity(speed_ * delta_time);
		if (boost_)
			velocity *= 5.0f;

		// Translate position by coresponding vector scaled by speed
		if (direction == Move::Right)
			position_ += right_ * velocity;

		else if (direction == Move::Left)
			position_ -= right_ * velocity;

		else if (direction == Move::Forward)
			position_ += front_ * velocity;

		else if (direction == Move::Backward)
			position_ -= front_ * velocity;

		else if (direction == Move::Up)
			position_ += up_ * velocity;

		else if (direction == Move::Down)
			position_ -= up_ * velocity;

		update_matrices();
	}

	// Perspective
	void Camera::zoom(const float& y_offset)
	{
		// Increment or decrement zoom based on 
		if (y_offset > 0.0f)
			fov_ -= 0.1f;
		else
			fov_ += 0.1f;

		// Keep zoom within reasonable bounds
		if (fov_ < 44.0f)
			fov_ = 44.0f;
		else if (fov_ > 46.0f)
			fov_ = 46.0f;

		// Keep camera updated
		update_matrices();
	}

	void Camera::zoom_reset()
	{
		fov_ = 45.0f;

		// Keep camera updated
		update_matrices();
	}

	///////////////////////////////////////////////////////////
	// UPDATE FUNCTIONS

	// Vectors
	void Camera::update_vectors()
	{
		// Calculate rotation matrix from euler angles
		const glm::mat4 rotation(glm::eulerAngleXYZ(pitch_, roll_, yaw_));

		// Apply rotation to world vectors
		right_ = glm::normalize(rotation * glm::vec4(world_right_, 1.0f));
		front_ = glm::normalize(rotation * glm::vec4(world_front_, 1.0f));
		up_ = glm::normalize(rotation * glm::vec4(world_up_, 1.0f));

		// Keep camera updated
		update_matrices();

		/*
		// Calculate front using euler angles
		front_ = glm::normalize(glm::vec3(
			cosf(pitch_) * sinf(yaw_),
			cosf(pitch_) * cosf(yaw_),
			sinf(pitch_)
		));

		// Calculate right using euler angles
		right_ = glm::normalize(glm::vec3(
			cosf(roll_) * cosf(yaw_),
			cosf(roll_) * sinf(yaw_),
			sinf(roll_)
		));

		// Calculate up using right and front
		up_ = glm::normalize(glm::cross(right_, front_));
		*/
	}

	// Matrices
	void Camera::update_matrices()
	{
		view_ = glm::lookAt(position_, position_ + front_, up_);
		projection_ = glm::perspective(fov_, aspect_, near_, far_);
	}

	///////////////////////////////////////////////////////
	// ATTRIBUTES
#
	// World
	const glm::vec3 Camera::world_right_ = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 Camera::world_front_ = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 Camera::world_up_ = glm::vec3(0.0f, 0.0f, 1.0f);
}
