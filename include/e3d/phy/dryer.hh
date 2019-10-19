#pragma once

#ifndef DRYER_H
#define DRYER_H

// GLM
#include <glm/glm.hpp>

// Project
#include "particle.hh"

class Dryer {
	glm::vec3 mPosition;
	glm::vec3 mDirection;
	float mAngle{};
	float mDistance{};
	float mSpeed{};

public:
	explicit Dryer(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& direction = glm::vec3(0.0f, 1.0f, 0.0f), const float& angle = 45.0f, const float& distance = 1.0f, const float& strength = 1.0f);

	float GetAngle() const;
	glm::vec3 GetDirection() const;

	void SetAngle(const float& angle);
	void SetDirection(const glm::vec3& direction);

	// Returns speed of wind at point p
	glm::vec3 Blow(const glm::vec3& position) const;
	glm::vec3 Blow(const Particle& particle) const;
	glm::vec3 Blow(const Particle& particle1, const Particle& particle2, const Particle& particle3) const;
};

#endif
