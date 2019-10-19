#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "rigidBody.hh"

class Sphere : public RigidBody {
public:
	float mRadius;

///public:
	explicit Sphere(const Mesh& mesh = Mesh(Mesh::Triangle));

	float GetRadius() const;

	void SetRadius(const float& radius);

	bool Collision(const Sphere& sphere) const;
	void Collide(Sphere& sphere);

	//void Integrate(const float& deltaTime);
};

#endif
