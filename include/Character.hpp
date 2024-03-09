#pragma once
#include <functional>
#include <SFML/Graphics.hpp>

#include "Animation.hpp"
#include "Physics/Body.hpp"
#include "KeyState.hpp"
#include "Physics/AABB.hpp"
#include "TileMap.hpp"
#include "Debug/Log.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Physics/Circle.hpp"

enum CharacterDirection {
	Left,
	Right
};


struct CharacterConfig {
	float health;
	float attack;
	float speed;
	float skillCooldown;
	float skillDuration;

	float scale = 1.0f;
	sf::Vector2f symmetryAxis{0.0f, 0.0f};
	sf::Vector2f size;
};

class Character: public Body {
public:
	Character(const CharacterConfig& config, PhysicsWorld* world);

	void update(float dt);

	void draw(sf::RenderTarget& window);

	const CharacterConfig& getConfig() const { return m_config; }

	CharacterDirection getDirection() const { return m_direction; }

	float windowTremble = 0.0f;
	bool deadAnimationPlaying = 0;
protected:
	AnimationManager m_animation;
	sf::Sprite m_sprite;

	sf::Vector2f m_lastPosition;
	CharacterDirection m_direction;

	CollisionDetail m_lastCollision;
	
	CharacterConfig m_config;

	PhysicsWorld* m_world = nullptr;

	void turnCharacter();
	void drawHealthBar(sf::RenderTarget& window) const;
	void drawSkillBar(sf::RenderTarget& window) const;

	virtual void onCollide(Body* other, const CollisionDetail& detail) override;
	virtual void afterCollisionTestCallback() override;
	virtual void myOnCollide(Body* other, const CollisionDetail& detail);
	virtual void myUpdate(float dt);
};