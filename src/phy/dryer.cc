#include "dryer.hh"

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/optimum_pow.hpp>


Dryer::Dryer(const glm::vec3& position, const glm::vec3& direction, const float& angle, const float& distance, const float& strength) : mPosition(position), mDirection(normalize(direction)), mAngle(glm::radians(angle)), mDistance(distance), mSpeed(strength) {}



float Dryer::GetAngle() const {
	return mAngle;
}
glm::vec3 Dryer::GetDirection() const {
	return mDirection;
}



void Dryer::SetAngle(const float& angle) {
	mAngle = glm::radians(angle);
}
void Dryer::SetDirection(const glm::vec3& direction) {
	mDirection = normalize(direction);
}



// Returns speed of wind at point p
glm::vec3 Dryer::Blow(const glm::vec3& position) const {
	const auto& vector = position - this->mPosition;

	if (glm::length(vector) > mDistance) {
		return glm::vec3(0.0f);
	}

	const auto& particleDirection = glm::normalize(vector);
	const auto& particleAngle = glm::clamp(glm::dot(particleDirection, glm::normalize(mDirection)), 0.0f, 1.0f);
	const auto& particleDistance = glm::length(vector);

	const auto& dryerAngle = cos(mAngle);
	if (particleAngle < dryerAngle) {
		return glm::vec3(0.0f);
	}

	const auto& strengthDistance = (mDistance - particleDistance) / mDistance;
	const auto& strengthAngle = glm::max((particleAngle - dryerAngle) / (1.0f - dryerAngle), 0.5f);
	const auto& strengthTotal = mSpeed * strengthDistance * strengthAngle;

	return particleDirection * strengthTotal;
}
glm::vec3 Dryer::Blow(const Particle& particle) const {
	const auto& vector = particle.GetPosition() - mPosition;

	if (glm::length(vector) > mDistance) {
		return glm::vec3(0.0f);
	}

	const auto& particleDirection = glm::normalize(vector);
	const auto& particleAngle = glm::clamp(glm::dot(particleDirection, glm::normalize(mDirection)), 0.0f, 1.0f);
	const auto& particleDistance = glm::length(vector);

	const auto& dryerAngle = cos(mAngle);
	if (particleAngle < dryerAngle) {
		return glm::vec3(0.0f);
	}

	const auto& strengthDistance = (mDistance - particleDistance) / mDistance;
	const auto& strengthAngle = glm::max((particleAngle - dryerAngle) / (1.0f - dryerAngle), 0.5f);
	const auto& strengthTotal = mSpeed * strengthDistance * strengthAngle;

	const auto& force = particleDirection * strengthTotal;
	const auto& drag = 0.5f * 1.225f * glm::pow2(glm::length(vector)) * particle.drag * particle.area * glm::normalize(vector);

	return force * drag;
}
glm::vec3 Dryer::Blow(const Particle& particle1, const Particle& particle2, const Particle& particle3) const {
	// Position of face
	const auto& point = (particle1.GetPosition() + particle2.GetPosition() + particle3.GetPosition()) / 3.0f;

	// Current vector from dryer to face
	const auto& vector = point - mPosition;

	// Return 0 if too far away
	if (glm::length(vector) > mDistance) {
		return glm::vec3(0.0f);
	}

	// Direction, angle and distance
	const auto& pDir = glm::normalize(vector);
	const auto& pAng = glm::clamp(glm::dot(pDir, glm::normalize(mDirection)), 0.0f, 1.0f);
	const auto& pDis = glm::length(vector);

	// Return 0 if out of bounds
	const auto& dAng = cos(mAngle);
	if (pAng < dAng) {
		return glm::vec3(0.0f);
	}

	// Find the strength
	const auto& sDis = (mDistance - pDis) / mDistance;
	const auto& sAng = glm::max((pAng - dAng) / (1.0f - dAng), 0.5f);
	const auto& sTotal = mSpeed * sDis * sAng;

	// Calculate force scaled with the strength based on angle [0..1] and distance [0..1]
	const auto& vWind = pDir * sTotal;

	const auto& vel = (particle1.velocity + particle2.velocity + particle3.velocity) / 3.0f - vWind;

	const auto& v12 = particle2.GetPosition() - particle1.GetPosition();
	const auto& v13 = particle3.GetPosition() - particle1.GetPosition();
	const auto& normal = glm::normalize(glm::cross(v12, v13));

	const auto& area = 0.5f * glm::length(glm::cross(v12, v13)) * glm::dot(vWind, normal) / glm::length(vel);

	const auto& drag = (particle1.drag + particle2.drag + particle3.drag) / 3;

	const auto& force = 0.5f * 1.225f * glm::pow2(glm::length(vel)) * drag * area * normal;

	return force / 3.0f;
}
