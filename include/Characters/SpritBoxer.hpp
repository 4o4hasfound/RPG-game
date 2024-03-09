#pragma once
#include <functional>
#include <unordered_map>

#include "KeyState.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Enemy.hpp"
#include "Character.hpp"

class SpritBoxer : public Character {
public:
	SpritBoxer(PhysicsWorld* world);

	sf::RenderWindow* window = nullptr;
private:
	static const CharacterConfig s_config;

	std::unordered_map<Body*, bool> m_hit;

	std::array<AABB, 4> m_attackRangeR{
		AABB{ sf::Vector2f{17.0f, 26.0f} *m_config.scale, sf::Vector2f{15.0f, 6.0} *m_config.scale },
		AABB{ sf::Vector2f{20.0f, 5.0f} *m_config.scale, sf::Vector2f{24.0f, 31.0} *m_config.scale },
		AABB{ sf::Vector2f{28.0f, 19.0f} *m_config.scale, sf::Vector2f{54.0f, 16.0} *m_config.scale },
		AABB{ sf::Vector2f{28.0f, 23.0f} *m_config.scale, sf::Vector2f{94.0f, 11.0} *m_config.scale }
	};

	std::array<AABB, 4> m_attackRangeL{
		AABB{ (sf::Vector2f{17.0f, 26.0f} - sf::Vector2f{15.0f, 0.0}) * m_config.scale, sf::Vector2f{15.0f, 6.0} *m_config.scale },
		AABB{ (sf::Vector2f{20.0f, 5.0f} - sf::Vector2f{24.0f, 0.0}) * m_config.scale, sf::Vector2f{24.0f, 31.0} *m_config.scale },
		AABB{ (sf::Vector2f{28.0f, 26.0f} - sf::Vector2f{54.0f, 0.0}) * m_config.scale, sf::Vector2f{54.0f, 16.0} *m_config.scale },
		AABB{ (sf::Vector2f{28.0f, 23.0f} - sf::Vector2f{94.0f, 0.0}) * m_config.scale, sf::Vector2f{94.0f, 11.0}*m_config.scale }
	};

	float m_timeToLastAtk;
	int m_nAtk;
	float m_dashCoolDown;
	float m_dashTimer;
	float m_trembleCounter;

	void initAnimation();
	virtual void myUpdate(float dt) override final;
	virtual void myOnCollide(Body* other, const CollisionDetail& detail) override final;

	AnimationType animationUpdate();

	void attack();
	void resetAttack();
};