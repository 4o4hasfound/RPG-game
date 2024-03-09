#pragma once
#include <vector>
#include <unordered_map>
#include <utility>
#include <type_traits>

#include <SFML/Graphics.hpp>

#include "FreeList.hpp"
#include "Physics/Body.hpp"
#include "Physics/Circle.hpp"
#include "Physics/Line.hpp"

class PhysicsWorld {
public:
	PhysicsWorld();
	~PhysicsWorld();

	void addBody(Body* body);
	int getIndex(Body* body) const;

	void update();
	void applyGravity(float dt);

	template<BodyType ToQuery = BodyType::None>
	std::vector<Body*> query(const AABB& aabb);

	template<BodyType ToQuery = BodyType::None>
	std::vector<Body*> query(const Circle& circle);

	template<BodyType ToQuery = BodyType::None>
	std::vector<Body*> query(const Line& circle);

	const AABB& getGlobalBound() const { return m_globalBound; }

	void renderAABB(sf::RenderTarget& window, const AABB& aabb);

	void clear();
private:
	std::vector<Body*> m_bodies;
	std::vector<AABB> m_aabbs;

	using Chunk = FreeList<int>;

	std::vector<std::vector<Chunk>> m_grid;
	float m_gravity = 0.006f;

	sf::Vector2f m_chunkSize = { 256 , 256 };
	AABB m_globalBound = { {-m_chunkSize.x, m_chunkSize.y}, m_chunkSize * 2.0f };

	void addObjectInGrid(int index, const AABB& aabb);
	void removeObjectInGrid(int index, const AABB& aabb);

	bool inBound(const AABB& aabb) const;

	void updateGrid();

	void expandGrid(const AABB& aabb);
	void expandGridHorizontal(const AABB& aabb);
	void expandGridVertical(const AABB& aabb);

	void globalCollisionDetection();

	template<typename T1, typename T2>
	struct PairHash {
		size_t operator()(const std::pair<T1, T2>& p) const {
			return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
		}
	};

	void localCollisionDetection(int bodyIndex, const Chunk& chunk, std::unordered_map<std::pair<int, int>, bool, PairHash<int, int> >& solved);
};