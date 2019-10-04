#pragma once

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

// GLM
#include <glm/glm.hpp>

// Project
#include "body.hh"
#include "../ogl/mesh.hh"

namespace phy
{

class RigidBody : public Body {
public:
	glm::mat3 mInverseInertia{};

///public:
	float density{};
	float friction{};
	glm::vec3 angularAcceleration{};
	glm::vec3 angularVelocity{};
	
	explicit RigidBody(const Mesh& mesh = Mesh(Mesh::Cube));

	glm::mat3 GetInverseInertia() const;

	void SetMass(const float& mass);
	void SetScale(const glm::mat4& scale);

	void Rotate(const float& angle, const glm::vec3& vector);
	void Scale(const glm::vec3& vector);

	void Update(const glm::vec3& force = glm::vec3(0));
	void Integrate(const float& deltaTime);

	glm::vec3 CalculateImpulse(const glm::vec3& position, const glm::vec3& normal);
	void ApplyImpulse(const glm::vec3& impulse, const glm::vec3& position, const glm::vec3& normal);

private:
	void CalculateInverseInertia();
};

}

#endif
