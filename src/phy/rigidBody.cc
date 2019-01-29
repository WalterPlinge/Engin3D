#include "rigidBody.hh"

// STD
#include <iostream>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <glm/gtx/matrix_cross_product.hpp>
#include <glm/gtx/optimum_pow.hpp>
#include <glm/gtx/orthonormalize.hpp>

RigidBody::RigidBody(const Mesh& mesh) : Body(mesh), density(1.0f) {
	CalculateInverseInertia();
}



glm::mat3 RigidBody::GetInverseInertia() const {
	return mMesh.rotate * glm::mat4(mInverseInertia) * glm::transpose(mMesh.rotate);
}



void RigidBody::SetMass(const float& mass) {
	mMass = mass;
	CalculateInverseInertia();
}
void RigidBody::SetScale(const glm::mat4& scale) {
	mMesh.scale = scale;
	CalculateInverseInertia();
}



void RigidBody::Rotate(const float& angle, const glm::vec3& vector) {
	mMesh.Rotate(angle, vector);
	CalculateInverseInertia();
}
void RigidBody::Scale(const glm::vec3& vector) {
	mMesh.Scale(vector);
	CalculateInverseInertia();
}



void RigidBody::Update(const glm::vec3& force) {
	mAcceleration = (CalculateForce() + force) / mMass;
}
void RigidBody::Integrate(const float& deltaTime) {
	// Integration
	velocity += mAcceleration * deltaTime;
	Translate(velocity * deltaTime);

	angularVelocity += angularAcceleration * deltaTime;
	
	// Create skew matrix for w
	const auto& w = glm::matrixCross3(angularVelocity);
	
	// Create 3x3 rotation matrix from rb rotation matrix
	glm::mat3 r = mMesh.rotate;

	// Update rotation matrix
	r += w * r * deltaTime;
	r = glm::orthonormalize(r);
	mMesh.rotate = r;
}

glm::vec3 RigidBody::CalculateImpulse(const glm::vec3& position, const glm::vec3& normal) {
	// Get velocity of impulse point
	const auto& vr = glm::vec3(0.0f) - (velocity + glm::cross(angularVelocity, position));

	// Calculate impulse
	// jr = (-(1 + e)v . n) / ((1 / m) + n . ((I(r x n)) x r))
	const auto& jNumerator = glm::dot(-(1.0f + elasticity) * vr, normal);
	const auto& jDenominator = 1.0f / mMass + glm::dot(normal, glm::cross(GetInverseInertia() * glm::cross(position, normal), position));

	// Impulse vector (impulse * normal)
	const auto& jn = jNumerator / jDenominator * normal;

	// WIP
	// Tangential impulse
	const auto& vt = vr - glm::dot(vr, normal) * normal;
	const auto& jt = -friction * glm::abs(glm::length(jn)) * glm::normalize(vt);

	glm::vec3 t;
	const auto& fe = CalculateForce();
	if (glm::dot(vr, normal) != 0.0f) {
		t = glm::normalize(jt);
	} else if (glm::dot(vr, normal) == 0.0f && glm::dot(fe, normal) != 0.0f) {
		t = glm::normalize(fe - glm::dot(fe, normal) * normal);
	} else {
		t = glm::vec3(0.0f);
	}

	glm::vec3 jf;
	if (glm::dot(vr, t) == 0.0f && glm::dot(mMass * vr, t) <= friction * glm::length(jn)) {
		jf = -glm::dot(mMass * vr, t) * t;
	} else {
		jf = -friction * glm::length(jn) * t;
	}

	auto jr = jn - jf;

	return jr;
}
void RigidBody::ApplyImpulse(const glm::vec3& impulse, const glm::vec3& position, const glm::vec3& normal) {
	const auto& e = glm::sqrt(elasticity);
	velocity -= impulse / mMass * e;
	angularVelocity -= -glm::length(impulse) * GetInverseInertia() * glm::cross(position, normal) * e;
}



void RigidBody::CalculateInverseInertia() {
	// Get minimum and maximum for mesh x, y and z
	const auto& dimensions = glm::vec3(mMesh.scale * glm::vec4(mMesh.GetDimensions(), 1.0f));

	auto inertia = glm::mat3(0.0f);
	inertia[0][0] = 1.0f / 12.0f * mMass * (glm::pow2(dimensions.y) + glm::pow2(dimensions.z));
	inertia[1][1] = 1.0f / 12.0f * mMass * (glm::pow2(dimensions.x) + glm::pow2(dimensions.z));
	inertia[2][2] = 1.0f / 12.0f * mMass * (glm::pow2(dimensions.x) + glm::pow2(dimensions.y));

	mInverseInertia = glm::inverse(inertia);

	//std::cout << glm::to_string(glm::inverse(inertia)) << std::endl;
}

