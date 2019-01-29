#ifndef PHY_FORCE_H
#define PHY_FORCE_H
#pragma once

///////////////////////////////////////////////////////////////
// HEADERS

// STD
#include <memory>

// GLM
#include <glm/vec3.hpp>

///////////////////////////////////////////////////////////////
// PHYSICS NAMESPACE

namespace phy {

	///////////////////////////////////////////////////////////
	// FORWARD DECLARATIONS

	class Body;

	///////////////////////////////////////////////////////////
	// FORCE

	class Force {
	public:
		Force();
		Force(const Force&);
		Force(Force&&) noexcept;
		
		virtual ~Force();
		
		Force& operator=(const Force&);
		Force& operator=(Force&&) noexcept;
		
		virtual glm::vec3 apply(const Body& body) = 0;
	};

	///////////////////////////////////////////////////////////
	// Gravity

	class Gravity : public Force {
	public:
		explicit Gravity(const glm::vec3& gravity = glm::vec3(0.0f, 0.0f, -9.8f));

		glm::vec3 apply(const Body& body) override;

	private:
		glm::vec3 gravity_;
	};

	///////////////////////////////////////////////////////////
	// Drag

	class Drag : public Force {
	public:
		glm::vec3 apply(const Body& body) override;
	};

	///////////////////////////////////////////////////////////
	// Hooke

	class Hooke : public Force {
	public:
		explicit Hooke(Body& body, const float& rest = 1.0f, const float& ks = 1.0f, const float& kd = 1.0f);

		glm::vec3 apply(const Body& body) override;

	private:
		std::shared_ptr<Body> target_;
		float rest_;
		float ks_;
		float kd_;
	};
}

#endif
