#include "force.hh"

///////////////////////////////////////////////////////////////
// HEADERS

// GLM
#include <glm/geometric.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/optimum_pow.hpp>

// Project
#include <phy/body.hh>

///////////////////////////////////////////////////////////////
// PHYSICS NAMESPACE

namespace phy {

	///////////////////////////////////////////////////////////
	// FORCE

	Force::Force() = default;
	Force::Force(const Force&) = default;
	Force::Force(Force&&) noexcept = default;
	
	Force::~Force() = default;
	
	Force& Force::operator=(const Force&) = default;
	Force& Force::operator=(Force&&) noexcept = default;

	///////////////////////////////////////////////////////////
	// Gravity

	Gravity::Gravity(const glm::vec3& gravity) : gravity_(gravity) {}

	glm::vec3 Gravity::apply(const Body& body)
	{
		return gravity_ * body.get_mass();
	}



	// Drag
	glm::vec3 Drag::apply(const Body& body)
	{
		const auto& p = (body.get_position() + mBody1->GetPos() + mBody2->GetPos()) / 3.0f;

		const auto& v = (body.velocity + mBody1->velocity + mBody2->velocity) / 3.0f - mDryer->Blow(p);

		const auto& v12 = mBody1->GetPos() - body.GetPos();
		const auto& v13 = mBody2->GetPos() - body.GetPos();
		const auto& n = glm::normalize(glm::cross(v12, v13));

		const auto& a = .5f * glm::length(glm::cross(v12, v13)) * glm::dot(v, n) / glm::length(v);

		if (glm::length(body.velocity) == 0.0f) {
			return glm::vec3(0.0f);
		}
		return 0.5f * 1.225f * glm::pow2(glm::length(body.velocity)) * body.drag * body.area * glm::normalize(body.velocity);
	}



	// Hooke
	Hooke::Hooke(Body& body, const float& rest, const float& ks, const float& kd) : target_(std::shared_ptr<Body>(&body)), kd_(kd), rest_(rest), ks_(ks) {}



	glm::vec3 Hooke::Apply(const Body& body)
	{
		// Vector between body and target
		const auto& v = target_->GetPosition() - body.GetPosition();

		// Return random unit vector if length is 0
		const auto& l = glm::length(v);
		if (l == 0.0f) {
			return glm::normalize(glm::vec3(float(rand())));
		}
		const auto& e = glm::normalize(v);

		// Linear velocities
		const auto& v1 = glm::dot(e, body.velocity);
		const auto& v2 = glm::dot(e, target_->velocity);

		// Coefficients
		const auto& fs = -ks_ * (rest_ - l);
		const auto& fd = -kd_ * (v1 - v2);

		// Total force
		const auto& f = fs + fd;
		return f * e;
	}
}
