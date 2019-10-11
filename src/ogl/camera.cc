#include "camera.hh"

#include <limits>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>



namespace ogl
{

auto const Camera::world_right_ = glm::vec3(1.0F, 0.0F, 0.0F);
auto const Camera::world_front_ = glm::vec3(0.0F, 1.0F, 0.0F);
auto const Camera::world_up_    = glm::vec3(0.0F, 0.0F, 1.0F);



//
Camera::Camera()
{
	// Keep camera updated
	update_vectors();
}



// Position
auto Camera::
position(
	) const
	-> glm::vec3
{
	return position_;
}

auto Camera::
position(
	glm::vec3 position
	)
	-> void
{
	position_ = position;
	update_matrices();
}



// Attributes
auto Camera::
fov(
	float fov
	)
	-> void
{
	fov_ = fov;
	update_matrices();
}

auto Camera::
aspect(
	float width,
	float height
	)
	-> void
{
	aspect_ = width / height;
	update_matrices();
}

auto Camera::
near(
	float near
	)
	-> void
{
	near_ = near;
	update_matrices();
}

auto Camera::
far(
	float far
	)
	-> void
{
	far_ = far;
	update_matrices();
}



// Angles
auto Camera::
pitch(
	float pitch
	)
	-> void
{
	pitch_ = glm::radians(pitch);
	update_vectors();
}

auto Camera::
roll(
	float roll
	)
	-> void
{
	roll_ = glm::radians(roll);
	update_vectors();
}

auto Camera::
yaw(
	float yaw
	)
	-> void
{
	yaw_ = glm::radians(yaw);
	update_vectors();
}



// Matrices
auto Camera::
projection(
	) const
	-> glm::mat4
{
	return projection_;
}

auto Camera::
view(
	) const
	-> glm::mat4
{
	return view_;
}



// Orientation
auto Camera::
aim(
	glm::vec2 look
	)
	-> void
{
	// Update pitch and yaw by look vector scaled by sensitivity
	auto const update = look * sensitivity;
	pitch_           += update.x;
	yaw_             += update.y;

	// Keep pitch within bounds
	auto const bounds =
		glm::half_pi<float>()
		- std::numeric_limits<float>::min();

	if (pitch_ > bounds)
		pitch_ = bounds;
	else if (pitch_ < -bounds)
		pitch_ = -bounds;
}

auto Camera::
aim(
	glm::vec3 look
	)
	-> void
{
	// Update pitch, roll and yaw by look vector scaled by sensitivity
	auto const update = look * sensitivity;
	pitch_           += update.x;
	roll_            += update.y;
	yaw_             += update.z;

	// Keep pitch and roll within bounds
	auto const bounds =
		glm::half_pi<float>()
		- std::numeric_limits<float>::min();

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

auto Camera::
look_at(
	glm::vec3 position
	)
	-> void
{
	// New front vector
	auto const front = glm::normalize(position - position_);

	// Get pitch and yaw from new front vector
	pitch_ = std::asin(-front.z);
	yaw_   = std::atan2(front.x, front.y);

	update_vectors();
}



// Movement
auto Camera::
move(
	Move  direction,
	float delta_time
	)
	-> void
{
	// Calculate velocity in relation to delta_time and apply boost
	auto velocity = speed * delta_time;
	if (boost)
		velocity *= 5.0F;

	// Translate position by coresponding vector scaled by speed
	if (direction == Right)
		position(right_ * velocity);

	else if (direction == Left)
		position(-right_ * velocity);

	else if (direction == Forward)
		position(front_ * velocity);

	else if (direction == Backward)
		position(-front_ * velocity);

	else if (direction == Up)
		position(up_ * velocity);

	else if (direction == Down)
		position(-up_ * velocity);

	update_matrices();
}



// Perspective
auto Camera::
zoom(
	float y_offset
	)
	-> void
{
	// Increment or decrement zoom based on
	if (y_offset > 0.0F)
		fov_ -= 0.1F;
	else
		fov_ += 0.1F;

	// Keep zoom within reasonable bounds
	if (fov_ < 44.0F)
		fov_ = 44.0F;
	else if (fov_ > 46.0F)
		fov_ = 46.0F;

	// Keep camera updated
	update_matrices();
}

auto Camera::
zoom_reset(
	)
	-> void
{
	fov_ = 45.0F;

	// Keep camera updated
	update_matrices();
}



// Update
auto Camera::
update_vectors(
	)
	-> void
{
	// Calculate rotation matrix from euler angles
	auto const rotation = glm::eulerAngleXYZ(pitch_, roll_, yaw_);

	// Apply rotation to world vectors
	right_ = glm::normalize(rotation * glm::vec4(world_right_, 1.0F));
	front_ = glm::normalize(rotation * glm::vec4(world_front_, 1.0F));
	up_    = glm::normalize(rotation * glm::vec4(world_up_, 1.0F));

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
auto Camera::
update_matrices(
	)
	-> void
{
	view_       = glm::lookAt(position_, position_ + front_, up_);
	projection_ = glm::perspective(fov_, aspect_, near_, far_);
}

} // namespace ogl
