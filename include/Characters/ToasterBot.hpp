#pragma once
#include <functional>
#include <unordered_map>

#include "KeyState.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Enemy.hpp"
#include "Character.hpp"

class ToasterBot : public Character {
public:
	ToasterBot(PhysicsWorld* world);
private:
	static const CharacterConfig s_config;

	const AABB m_attackRangeR{ sf::Vector2f{7.0f + m_config.symmetryAxis.x, 0.0f} *m_config.scale, sf::Vector2f{82.f, 20.0} *m_config.scale };
	const AABB m_attackRangeL{ (sf::Vector2f{-7.0f + m_config.symmetryAxis.x, 0.0f} - sf::Vector2f{82.f, 0.0}) * m_config.scale, sf::Vector2f{82.f, 20.0} *m_config.scale };

	std::unordered_map<Body*, bool> m_hit;

	void initAnimation();
	virtual void myUpdate(float dt) override final;

	AnimationType animationUpdate();

	void attack();
	void resetAttack();
};