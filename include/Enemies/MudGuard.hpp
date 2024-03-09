#pragma once
#include <functional>

#include "KeyState.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Enemy.hpp"
#include "Physics/Line.hpp"

class MudGuard : public Enemy {
public:
	MudGuard(PhysicsWorld* world);

private:
	static const EnemyConfig s_config;
public:
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
	const std::array<AABB, 2> m_attackRangeR{ 
		AABB{ sf::Vector2f{23.0f, 13.0f} *s_config.scale, sf::Vector2f{20.f, 14.0f} *s_config.scale },
		AABB{ sf::Vector2f{25.0f, 10.0f} *s_config.scale, sf::Vector2f{56.0f, 9.0f} *s_config.scale }
	};
	const std::array<AABB, 2> m_attackRangeL{
		AABB{ sf::Vector2f{23.0f * -1.0f + s_config.symmetryAxis.x * 2.0f - 20.f, 13.0f} *s_config.scale, sf::Vector2f{20.f, 14.0f} *s_config.scale },
		AABB{ sf::Vector2f{25.0f * -1.0f + s_config.symmetryAxis.x * 2.0f - 56.0f, 10.0f} *s_config.scale, sf::Vector2f{56.0f, 9.0f} *s_config.scale }
	};
	int m_nAtk;
	Line m_line;
private:

	std::unordered_map<Body*, bool> m_hit;
	std::vector<sf::Vector2f> m_path;

	float m_timeToLastAtk;

	virtual void myUpdate(float dt) override final;
	virtual void myOnCollide(Body* other, const CollisionDetail& detail) override;

	void initAnimation();
	AnimationType animationUpdate();

	void attack();
	void resetAttack();
};