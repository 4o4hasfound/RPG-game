#pragma once

#include "Physics/PhysicsWorld.hpp"
#include "Animation.hpp"
#include "Physics/Body.hpp"
#include "Character.hpp"

struct EnemyConfig {
	float health;
	float attack;
	float speed;

	float scale = 1.0f;
	sf::Vector2f symmetryAxis{ 0.0f, 0.0f };
	sf::Vector2f size;
};

class Enemy : public Body {
public:
	Enemy(const EnemyConfig& config, PhysicsWorld* world);

	void update(float dt);

	void draw(sf::RenderTarget& window);

	CharacterDirection getDirection() const { return m_direction; }
	void setDirection(CharacterDirection direction) { m_direction = direction; }
protected:
	AnimationManager m_animation;
	EnemyConfig m_config;

	sf::Sprite m_sprite;

	sf::Vector2f m_lastPosition;
	CharacterDirection m_direction;

	CollisionDetail m_lastCollision;

	PhysicsWorld* m_world = nullptr;

	void turnCharacter();
	void drawHealthBar(sf::RenderTarget& window) const;

	virtual void onCollide(Body* other, const CollisionDetail& detail) override;
	virtual void afterCollisionTestCallback() override;
	virtual void myUpdate(float dt);
	virtual void myOnCollide(Body* other, const CollisionDetail& detail);
};