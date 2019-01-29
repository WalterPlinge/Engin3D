#ifndef OGL_CAMERA_H
#define OGL_CAMERA_H
#pragma once

///////////////////////////////////////////////////////////////
// HEADERS

// GLM
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

///////////////////////////////////////////////////////////////
// RENDER NAMESPACE

namespace ogl {

	///////////////////////////////////////////////////////////
	// CAMERA CLASS

	class Camera {
	public:

		///////////////////////////////////////////////////////
		// DIRECTIONS OF MOVEMENT

		enum class Move {
			Right,
			Left,
			Forward,
			Backward,
			Up,
			Down
		};

		///////////////////////////////////////////////////////
		// CONSTRUCTOR

		explicit Camera();

		///////////////////////////////////////////////////////
		// GETTERS

		// Variables
		glm::vec3 get_position() const;

		// Matrices
		glm::mat4 get_projection() const;
		glm::mat4 get_view() const;

		///////////////////////////////////////////////////////
		// SETTERS

		// Variables
		void set_position(const glm::vec3& position);
		void set_sensitivity(const float& sensitivity);
		void set_speed(const float& speed);
		void set_boost(const bool& boost);

		// Attributes
		void set_fov(const float& fov);
		void set_aspect(const float& width, const float& height);
		void set_near(const float& near);
		void set_far(const float& far);

		// Angles
		void set_pitch(const float& pitch);
		void set_roll(const float& roll);
		void set_yaw(const float& yaw);

		///////////////////////////////////////////////////////
		// MOVEMENT FUNCTIONS

		// Orientation
		void aim(const glm::vec2& look);
		void aim(const glm::vec3& look);
		void look_at(const glm::vec3& position);

		// Position
		void move(const Move& direction, const float& delta_time);

		// Perspective
		void zoom(const float& y_offset);
		void zoom_reset();

	private:

		///////////////////////////////////////////////////////
		// UPDATE FUNCTIONS

		void update_vectors();
		void update_matrices();

		///////////////////////////////////////////////////////
		// ATTRIBUTES

		// Variables
		glm::vec3 position_ = glm::vec3(0.0f);
		float sensitivity_ = 0.25f;
		float speed_ = 10.0f;
		bool boost_ = false;

		// Attributes
		float fov_ = 45.0f;
		float aspect_ = 1.0f;
		float near_ = 0.01f;
		float far_ = 100.0f;

		// Angles
		float pitch_ = 0.0f;
		float roll_ = 0.0f;
		float yaw_ = 0.0f;

		// Local vectors
		glm::vec3 right_ = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 front_ = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 up_ = glm::vec3(0.0f, 0.0f, 1.0f);

		// World vectors
		static const glm::vec3 world_right_;
		static const glm::vec3 world_front_;
		static const glm::vec3 world_up_;

		// Matrices
		glm::mat4 projection_ = glm::mat4(1.0f);
		glm::mat4 view_ = glm::mat4(1.0f);
	};
}

#endif
