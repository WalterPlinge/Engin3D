#include "particle.hh"

// GLM
#include <glm/gtc/constants.hpp>

Particle::Particle(const Mesh& mesh) : Body(mesh) {
	area = 0.01f;
	drag = 1.05f;
	elasticity = 0.9f;
	mMass = 0.1f;
	Rotate(glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
	Scale(glm::vec3(0.1f));
}



void Particle::Update(const glm::vec3& force) {
	mAcceleration = CalculateForce() + force / mMass;
}
void Particle::Integrate(const float& deltaTime, const Integration& type) {
	if (type == SemiImplicit) {
		velocity += mAcceleration * deltaTime;
		SetPosition(GetPosition() + velocity * deltaTime);
	} else if(type == Forward) {
		SetPosition(GetPosition() + velocity * deltaTime);
		velocity += mAcceleration * deltaTime;
	}
}
