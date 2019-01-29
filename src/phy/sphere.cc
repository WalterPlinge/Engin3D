#include "sphere.hh"

// GLM
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/optimum_pow.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/fast_square_root.hpp>


Sphere::Sphere(const Mesh& mesh) : RigidBody(mesh), mRadius(1.0f) {}



float Sphere::GetRadius() const {
	return mRadius;
}



void Sphere::SetRadius(const float& radius) {
	const auto& scale = radius - mRadius;
	mRadius = radius;
	Scale(glm::vec3(scale));
}



bool Sphere::Collision(const Sphere& sphere) const {
	///return glm::length2(sphere.GetPosition() - GetPosition()) <= glm::pow2(sphere.mRadius + mRadius);
	return glm::length2(sphere.mPosition - mPosition) <= 4.0f;
}
void Sphere::Collide(Sphere& sphere) {
	// resolve overlap
	const auto vector(sphere.mPosition - mPosition);

	///if (glm::length2(vector) > glm::pow2(sphere.mRadius + mRadius)) {
	if (glm::length2(vector) > 4.0f) {
		return;
	}

	const auto distance(glm::fastLength(vector));
	///const auto& overlap = sphere.mRadius + mRadius - distance;
	const auto overlap(2.0f - distance);
	const auto normal(vector / distance);
	///Translate(-overlap * normal);
	mPosition -= overlap * normal;

	///vector = normal * (overlap + distance);
	///vector = normal * 2.0f;


	// Calculate impulses
	///const auto& relativeVelocity = sphere.velocity - velocity;

	// Short version
	///const auto& j = glm::dot(-(1 + elasticity) * relativeVelocity, normal) / (1.0f / GetMass() + 1.0f / sphere.GetMass());
	const auto& j = glm::dot(-2.0f * (sphere.velocity - velocity), normal) / 2.0f;
	///velocity -= j / GetMass() * normal * elasticity;
	const auto impulse(j * normal);
	velocity -= impulse;
	///sphere.velocity += j / sphere.GetMass() * normal * sphere.elasticity;
	sphere.velocity += impulse;

	// Long version
	/*
	const auto& contact = GetPosition() + normal * GetRadius();
	const auto& r1 = contact - GetPosition();
	const auto& r2 = contact - sphere.GetPosition();
	
	const auto& numerator = glm::dot(-(1 + elasticity) * relativeVelocity, normal);
	const auto& denominator = (1.0f / GetMass()) + (1.0f / sphere.GetMass()) + glm::dot(normal, glm::cross(GetInverseInertia() * glm::cross(r1, normal), r1) + glm::cross(sphere.GetInverseInertia() * glm::cross(r2, normal), r2));

	const auto& impulse = numerator / denominator;

	// Apply impulses
	velocity -= impulse / mMass * normal * elasticity;
	sphere.velocity += impulse / sphere.mMass * normal * sphere.elasticity;
	*/
}



//void Sphere::Integrate(const float& deltaTime) {
//	velocity += mAcceleration * deltaTime;
//	Translate(velocity * deltaTime);
//}
