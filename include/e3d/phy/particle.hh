#pragma once

#ifndef PARTICLE_H
#define PARTICLE_H

// GLM
#include <glm/glm.hpp>

// Project
#include "body.hh"
#include "../ogl/mesh.hh"

class Particle : public Body {
public:
	enum Integration {
		Forward,
		SemiImplicit
	};

	explicit Particle(const Mesh& mesh = Mesh(Mesh::Quad));

	void Update(const glm::vec3& force = glm::vec3(0.0f));
	void Integrate(const float& deltaTime, const Integration& type = SemiImplicit);
};

#endif
