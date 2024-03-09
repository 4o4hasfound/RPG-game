#pragma once
#include <functional>
#include <unordered_map>

#include "KeyState.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Enemy.hpp"
#include "Character.hpp"

class BACBot : public Character {
public:
	BACBot(PhysicsWorld* world);

	sf::RenderWindow* window = nullptr;

private:
	static const CharacterConfig s_config;

	std::unordered_map<Body*, bool> m_hit;

	std::array<AABB, 2> m_attackRangeR{
		AABB{ sf::Vector2f{58.0f, 16.0f} *m_config.scale, sf::Vector2f{63.0f, 20.0} *m_config.scale },
		AABB{ sf::Vector2f{53.0f, 17.0f} *m_config.scale, sf::Vector2f{83.0f, 19.0} *m_config.scale }
	};

	std::array<AABB, 2> m_attackRangeL{
		AABB{ (sf::Vector2f{58.0f * -1.0f + s_config.symmetryAxis.x * 2.0f, 16.0f} - sf::Vector2f{63.0f, 0.0}) * m_config.scale, sf::Vector2f{63.0f, 20.0} *m_config.scale },
		AABB{ (sf::Vector2f{53.0f * -1.0f + s_config.symmetryAxis.x * 2.0f, 17.0f} - sf::Vector2f{83.0f, 0.0}) * m_config.scale, sf::Vector2f{83.0f, 19.0} *m_config.scale }
	};
	int m_nAtk;

	float m_timeToLastAtk;
	float m_dashCoolDown;
	float m_dashTimer;

	void initAnimation();
	virtual void myUpdate(float dt) override final;
	virtual void myOnCollide(Body* other, const CollisionDetail& detail) override final;

	AnimationType animationUpdate();

	void attack();
	void resetAttack();
};