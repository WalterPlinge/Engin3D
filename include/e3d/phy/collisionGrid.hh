#pragma once

#ifndef COLLISIONGRID_H
#define COLLISIONGRID_H

// STD
#include <map>
#include <memory>
#include <set>
#include <vector>

// GLM
#include <glm/glm.hpp>

// Project
#include "sphere.hh"

/*
 *	Collision grid
 *	
 *	-vec3 position
 *	-vec3 size
 *	-vec3 cell size
 *	-uvec3 grid size
 *	-map (unsigned, set (pointer (sphere))) cells
 *	-map (pointer (sphere), set (unsigned)) spheres
 *	
 *	+get
 *	+set
 *	+add (sphere)
 *	+clear
 *	+grid position (unsigned or uvec3) 
 *	-calculate grid size
 */

class CollisionGrid {
public:
	glm::vec2 mPosition;
	glm::vec2 mSize;
	glm::vec2 mCellSize;
	glm::uvec2 mGridSize;
	std::map<unsigned, std::set<Sphere*>> mCells;
	std::map<Sphere*, std::set<unsigned>> mSpheres;

	void CalculateGridSize();
///public:

	CollisionGrid();

	glm::vec2 GetPosition() const;
	glm::vec2 GetSize() const;
	glm::vec2 GetCellSize() const;
	glm::uvec2 GetGridSize() const;

	void SetPosition(const glm::vec2& position);
	void SetSize(const glm::vec2& size);
	void SetCellSize(const glm::vec2& cellSize);

	void Add(Sphere& sphere);
	void Add(std::vector<Sphere>& spheres);

	void Update();

	void Collide();

	void Clear();

	unsigned GetGridIndex(const glm::uvec2& pos) const;
	glm::uvec2 GetGridPosition(const unsigned& pos) const;
	glm::uvec2 GetGridPosition(const glm::vec2& pos) const;
};

#endif
