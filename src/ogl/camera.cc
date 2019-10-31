#include "e3d/ogl/camera.hh"

#include <limits>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>



namespace ogl
{

glm::vec3 const Camera::world_right_ = glm::vec3(1.0F, 0.0F, 0.0F);
glm::vec3 const Camera::world_front_ = glm::vec3(0.0F, 1.0F, 0.0F);
glm::vec3 const Camera::world_up_    = glm::vec3(0.0F, 0.0F, 1.0F);



// Constructor
Camera::Camera()
{
	// Keep camera updated
	update_vectors();
}



// Attributes
auto Camera::
aspect(
	float const width,
	float const height
	)
	-> void
{
	aspect_ = width / height;
}



// Matrices
auto Camera::
projection(
	) const
	-> glm::mat4
{
	return glm::perspective(glm::radians(fov), aspect_, near, far);
}

auto Camera::
view(
	) const
	-> glm::mat4
{
	return glm::lookAt(position, position + front_, up_);
}



// Orientation
auto Camera::
aim(
	glm::vec2 const look
	)
	-> void
{
	// Update pitch and yaw by look vector scaled by sensitivity
	auto const update = look * sensitivity;
	pitch_           += update.y;
	yaw_             += update.x;

	// Keep pitch within bounds
	auto const bounds =
		glm::half_pi<float>()
		- std::numeric_limits<float>::min();

	if (pitch_ > bounds)
		pitch_ = bounds;
	else if (pitch_ < -bounds)
		pitch_ = -bounds;

	update_vectors();
}

auto Camera::
look_at(
	glm::vec3 const point
	)
	-> void
{
	// New front vector
	auto const front = glm::normalize(point - position);

	// Get pitch and yaw from new front vector
	pitch_ = std::asin(front.z);
	yaw_   = std::atan2(front.x, front.y);

	update_vectors();
}



// Movement
auto Camera::
move(
	Move  const direction,
	float const delta_time
	)
	-> void
{
	// Calculate velocity in relation to delta_time and apply boost
	auto velocity = speed;
	if (boost)
		velocity *= 5.0F;
	velocity *= delta_time;

	// Translate position by coresponding vector scaled by speed
	if (direction == Right)
		position += right_ * velocity;

	else if (direction == Left)
		position -= right_ * velocity;

	else if (direction == Forward)
		position += front_ * velocity;

	else if (direction == Backward)
		position -= front_ * velocity;

	else if (direction == Up)
		position += up_ * velocity;

	else if (direction == Down)
		position -= up_ * velocity;
}



// Perspective
auto Camera::
zoom(
	float const y_offset
	)
	-> void
{
	// Increment or decrement zoom based on y_offset sign
	if (y_offset > 0.0F)
		fov -= 1.0F;
	else
		fov += 1.0F;

	// Keep zoom within reasonable bounds
	if (fov < 20.0F)
		fov = 20.0F;
	else if (fov > 90.0F)
		fov = 90.0F;
}

auto Camera::
zoom_reset(
	)
	-> void
{
	fov = 45.0F;
}



// Update
auto Camera::
update_vectors(
	)
	-> void
{
	auto front = glm::vec3();
	front.x    = glm::sin(yaw_) * glm::cos(pitch_);
	front.y    = glm::cos(yaw_) * glm::cos(pitch_);
	front.z    = glm::sin(pitch_);

	front_ = glm::normalize(front);
	right_ = glm::normalize(glm::cross(front_, world_up_));
	up_    = glm::normalize(glm::cross(right_, front_));

	/*
	// Calculate rotation matrix from euler angles
	auto const rotation = glm::eulerAngleXYZ(pitch_, roll_, yaw_);

	// Apply rotation to world vectors
	right_ = glm::normalize(rotation * glm::vec4(world_right_, 1.0F));
	front_ = glm::normalize(rotation * glm::vec4(world_front_, 1.0F));
	up_    = glm::normalize(rotation * glm::vec4(world_up_, 1.0F));

	// Keep camera updated
	update_matrices();
	*/

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

} // namespace ogl
