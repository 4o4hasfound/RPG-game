#pragma once
#include <functional>

#include "KeyState.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Enemy.hpp"
#include "Physics/Line.hpp"

class DroidZapper : public Enemy {
public:
	DroidZapper(PhysicsWorld* world);

private:
	static const EnemyConfig s_config;
public:
	const AABB m_feetR{ 
		sf::Vector2f{s_config.symmetryAxis + 0.5f * s_config.size} * s_config.scale,
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
	const AABB m_attackRangeR{ sf::Vector2f{7.0f + s_config.symmetryAxis.x, 21.0f} *s_config.scale, sf::Vector2f{36.f, 13.0} *s_config.scale };
	const AABB m_attackRangeL{ (sf::Vector2f{-7.0f + s_config.symmetryAxis.x, 21.0f} - sf::Vector2f{36.f, 0.0}) * s_config.scale, sf::Vector2f{36.f, 13.0} *s_config.scale };
	Line m_line;
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