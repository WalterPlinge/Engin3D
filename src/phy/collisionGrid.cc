#include "collisionGrid.hh"

// STD
#include <algorithm>
#include <set>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/optimum_pow.hpp>



// Constructor
CollisionGrid::CollisionGrid() : mPosition(0.0f), mSize(1.0f), mCellSize(1.0f), mGridSize(1) {}



// Getters
glm::vec2 CollisionGrid::GetPosition() const {
	return mPosition;
}
glm::vec2 CollisionGrid::GetSize() const {
	return mSize;
}
glm::vec2 CollisionGrid::GetCellSize() const {
	return mCellSize;
}
glm::uvec2 CollisionGrid::GetGridSize() const {
	return mGridSize;
}



// Setters
void CollisionGrid::SetPosition(const glm::vec2& position) {
	mPosition = position;
	CalculateGridSize();
}
void CollisionGrid::SetSize(const glm::vec2& size) {
	mSize = size;
	CalculateGridSize();
}
void CollisionGrid::SetCellSize(const glm::vec2& cellSize) {
	mCellSize = cellSize;
	CalculateGridSize();
}



// Add sphere to grid
void CollisionGrid::Add(Sphere& sphere) {
	// 2D position of sphere
	glm::vec2 position(sphere.mPosition.x, sphere.mPosition.z);
	const float& radius(sphere.mRadius);

	// 2 corner positions
	if (position.x < 1.0f) {
		position.x = 1.0f;
	}
	if (position.y < 1.0f) {
		position.y = 1.0f;
	}
	const glm::uvec2 p1(GetGridPosition(position - radius));
	const glm::uvec2 p2(GetGridPosition(position + radius));

	// Find all grid spaces
	std::set<unsigned>& positions(mSpheres[&sphere]);
	for (auto x = p1.x; x <= p2.x; ++x) {
		for (auto y = p1.y; y <= p2.y; ++y) {
			const unsigned pos = GetGridIndex({ x, y });
			bool found = false;
			for (auto& p : positions) {
				if (p == pos) {
					found = true;
					break;
				}
			}
			if (!found) {
				positions.insert(GetGridIndex({ x, y }));
			}
		}
	}

	// Add sphere to cells
	for (auto& p : positions) {
		mCells[p].insert(&sphere);
	}
}
void CollisionGrid::Add(std::vector<Sphere>& spheres) {
	for (Sphere& s : spheres) {
		Add(s);
	}
}



void CollisionGrid::Update() {
	for (auto& s : mSpheres) {
		Sphere& sphere(*s.first);

		// 2D position of sphere
		glm::vec2 position(glm::abs(sphere.mPosition.x), glm::abs(sphere.mPosition.z));
		/*if (position.x < 1.0f) {
			position.x = 1.0f;
		}
		if (position.y < 1.0f) {
			position.y = 1.0f;
		}*/
		const float& radius(sphere.mRadius);

		// 2 corner positions
		const glm::uvec2 p1(GetGridPosition(position - radius));
		const glm::uvec2 p2(GetGridPosition(position + radius));

		std::set<unsigned>& positions(s.second);
		std::set<unsigned> difference(positions);
		positions.clear();
		if (p1 == p2) {
			positions.insert(GetGridIndex(p1));
		} else {
			for (unsigned x = p1.x; x <= p2.x; ++x) {
				for (unsigned y = p1.y; y <= p2.y; ++y) {
					positions.insert(GetGridIndex({ x, y }));
				}
			}
		}

		// Remove from old cells
		for (const unsigned& d : difference) {
			if (positions.find(d) == positions.end()) {
				auto& cell(mCells[d]);
				cell.erase(&sphere);
				if (cell.empty()) {
					mCells.erase(d);
				}
			}
		}

		// Add to new cells
		for (const unsigned& p : positions) {
			if (difference.find(p) == difference.end()) {
				mCells[p].insert(&sphere);
			}
		}
	}

	// Remove empty cells
	//for (auto c = mCells.begin(); c != mCells.end(); ++c) {
	//	if (c->second.empty()) {
	//		mCells.erase(c--);
	//	}
	//}
}



// Collisions
void CollisionGrid::Collide() {
	// Resolve collisions in each cell
	for (auto& cell : mCells) {
		// List of spheres in cell
		const auto& spheres(cell.second);

		// Collide balls with each other
		/*
		 *	If list has 2 or more spheres
		 *		For each sphere
		 *			For the rest
		 *				Collide
		 */
		if (spheres.size() > 1) {
			for (auto i = spheres.begin(); i != spheres.end(); ++i) {
				for (auto j = std::next(i); j != spheres.end(); ++j) {
					(**i).Collide(**j);
				}
			}
		}

		// Collision with walls
		/*
		 *	Each position on the edge
		 *		For each sphere
		 *			Test against wall
		 *				Resolve position
		 *				Invert velocity
		 */
		const glm::uvec2 pos(GetGridPosition(cell.first));
		if (pos.x == 0) {
			for (auto& s : spheres) {
				///const float bound(mPosition.x + s->mRadius);
				///if (s->mPosition.x < bound) {
				if (s->mPosition.x < 1.0f) {
					///s->mPosition.x = bound;
					s->mPosition.x = 1.0f;
					///s->velocity.x *= -s->elasticity;
					s->velocity.x *= -1.0f;
				}
			}
		} else if (pos.x == mGridSize.x - 1) {
			for (auto& s : spheres) {
				const float bound(mPosition.x + mSize.x - s->mRadius);
				if (s->mPosition.x > bound) {
				///if (s->mPosition.x > 999.0f) {
					s->mPosition.x = bound;
					///s->mPosition.x = 999.0f;
					///s->velocity.x *= -s->elasticity;
					s->velocity.x *= -1.0f;
				}
			}
		}
		if (pos.y == 0) {
			for (auto& s : spheres) {
				///const float bound(mPosition.y + s->mRadius);
				///if (s->mPosition.z < bound) {
				if (s->mPosition.z < 1.0f) {
					///s->mPosition.z = bound;
					s->mPosition.z = 1.0f;
					///s->velocity.z *= -s->elasticity;
					s->velocity.z *= -1.0f;
				}
			}
		} else if (pos.y == mGridSize.y - 1) {
			for (auto& s : spheres) {
				const float bound(mPosition.y + mSize.y - s->mRadius);
				if (s->mPosition.z > bound) {
				///if (s->mPosition.z > 999.0f) {
					s->mPosition.z = bound;
					///s->mPosition.z = 999.0f;
					///s->velocity.z *= -s->elasticity;
					s->velocity.z *= -1.0f;
				}
			}
		}
	}
}



// Clear function
void CollisionGrid::Clear() {
	mCells.clear();
	mSpheres.clear();
}



// Get grid position
unsigned CollisionGrid::GetGridIndex(const glm::uvec2& pos) const {
	return pos.x + pos.y * mGridSize.x;
}
glm::uvec2 CollisionGrid::GetGridPosition(const unsigned& pos) const {
	const auto& x = pos % mGridSize.x;
	return glm::uvec2(x, (pos - x) / mGridSize.x);
}
glm::uvec2 CollisionGrid::GetGridPosition(const glm::vec2& pos) const {
	/*glm::vec2 p = pos;
	if (p.x < 0.0f) {
		p.x = 0.0f;
	}
	if (p.y < 0.0f) {
		p.y = 0.0f;
	}*/
	return glm::uvec2(pos / mCellSize);
}



// Calculate the grid size based on size and cell size
void CollisionGrid::CalculateGridSize() {
	mGridSize = glm::round(mSize / mCellSize);
}
