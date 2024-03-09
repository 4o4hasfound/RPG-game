#pragma once

#pragma once
#include <functional>

#include "KeyState.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Enemy.hpp"
#include "Physics/Line.hpp"

class StormHead : public Enemy {
public:
	StormHead(PhysicsWorld* world);

private:
	static const EnemyConfig s_config;

	const AABB m_feetR{
		sf::Vector2f{s_config.symmetryAxis + 0.5f * s_config.size} *s_config.scale,
		sf::Vector2f{10.0f, 10.0f}
	};
	const AABB m_feetL{
		sf::Vector2f{s_config.symmetryAxis + 0.5f * sf::Vector2f{-s_config.size.x, s_config.size.y}} *s_config.scale - sf::Vector2f{10.0f, 0.0f},
		sf::Vector2f{10.0f, 10.0f}
	};
	const AABB m_right{
		sf::Vector2f{s_config.symmetryAxis + 0.5f * sf::Vector2f{s_config.size.x, -s_config.size.y}} *s_config.scale,
		sf::Vector2f{10.0f, s_config.size.y}
	};
	const AABB m_left{
		sf::Vector2f{s_config.symmetryAxis + 0.5f * sf::Vector2f{-s_config.size.x, -s_config.size.y}} *s_config.scale - sf::Vector2f{10.0f, 0.0f},
		sf::Vector2f{10.0f, s_config.size.y}
	};
	const Circle m_playerDetect{ s_config.symmetryAxis * s_config.scale, 200.0f * s_config.scale };
	const std::array<std::vector<AABB>, 2> m_attackRangeR{
		std::vector<AABB>{
			{sf::Vector2f{48.0f, 51.0f} *s_config.scale, sf::Vector2f{26.0f, 27.0f} *s_config.scale},
			{sf::Vector2f{74.0f, 59.0f} *s_config.scale, sf::Vector2f{18.0f, 42.0f} *s_config.scale},
			{sf::Vector2f{89.0f, 84.0f} *s_config.scale, sf::Vector2f{24.0f, 39.0f} *s_config.scale}
		} ,
		std::vector<AABB>{
			{sf::Vector2f{37.0f, 58.0f} *s_config.scale, sf::Vector2f{22.0f, 24.0f} *s_config.scale},
			{sf::Vector2f{24.0f, 73.0f} *s_config.scale, sf::Vector2f{13.0f, 12.0f} *s_config.scale},
			{sf::Vector2f{6.0f, 76.0f} *s_config.scale, sf::Vector2f{18.0f, 47.0f} *s_config.scale}
		}
	};
	const std::array<std::vector<AABB>, 2> m_attackRangeL{
		std::vector<AABB>{
			{sf::Vector2f{48.0f * -1.0f + s_config.symmetryAxis.x * 2.0f - 26.0f, 51.0f} *s_config.scale, sf::Vector2f{26.0f, 27.0f} *s_config.scale},
			{sf::Vector2f{74.0f * -1.0f + s_config.symmetryAxis.x * 2.0f - 18.0f, 59.0f} *s_config.scale, sf::Vector2f{18.0f, 42.0f} *s_config.scale},
			{sf::Vector2f{89.0f * -1.0f + s_config.symmetryAxis.x * 2.0f - 24.0f, 84.0f} *s_config.scale, sf::Vector2f{24.0f, 39.0f} *s_config.scale}
		} ,
		std::vector<AABB>{
			{sf::Vector2f{37.0f - 22.0f, 58.0f} *s_config.scale, sf::Vector2f{22.0f, 24.0f} *s_config.scale},
			{sf::Vector2f{24.0f - 13.0f, 73.0f} *s_config.scale, sf::Vector2f{13.0f, 12.0f} *s_config.scale},
			{sf::Vector2f{6.0f - 18.0f, 76.0f} *s_config.scale, sf::Vector2f{18.0f, 47.0f} *s_config.scale}
		}
	};
	Line m_line;

	int m_nAtk;
private:

	std::unordered_map<Body*, bool> m_hit;
	std::vector<sf::Vector2f> m_path;

	virtual void myUpdate(float dt) override final;
	virtual void myOnCollide(Body* other, const CollisionDetail& detail) override;

	void initAnimation();
	AnimationType animationUpdate();

	void attack();
	void resetAttack();
};