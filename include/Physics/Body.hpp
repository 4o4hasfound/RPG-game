#pragma once
#include <functional>
#include <SFML/Graphics.hpp>

#include "Physics/AABB.hpp"
#include "Debug/Log.hpp"
		
enum class BodyType {
	Static,
	Static_Uncollidable,
	Character,
	Enemy,
	None
};

struct BodyStatus {
	bool moving = 0;
	bool inSkill = 0;
	bool attacking = 0;
	bool onGround = 1; 
	bool onWall = 0;
	bool hasTouchWall = 0; // just some flag to change the onWall after the collision test. 
	bool dead = 0;
	bool hit = 0;

	float health;
	float attack;

	float skillCooldown;
	float skillDuration;

	bool shocked = 0;
	float shockDuration;

	sf::Vector2f velocity = {0.0f, 0.0f};
};

struct CollisionDetail {
	sf::Vector2f direction;
	float delta;
};


class Body {
	friend class PhysicsWorld;
public:
	Body(BodyType type, AABB aabb = {0.0f, 0.0f, 0.0f, 0.0f});
	virtual ~Body();


	const AABB& getAABB() const { return m_aabb; }
	BodyType getType() const { return m_type; }

	bool getFreezed() const { return m_freezed; }
	void setFreezed(bool freezed) { m_freezed = freezed; }

	BodyStatus& getStatus() { return m_status; }

	sf::Vector2f getPosition() const { return m_position; }
	void setPosition(const sf::Vector2f position) { 
		m_position += position - m_aabb.getPosition();
		m_aabb.setPosition(position);
	}
	void move(const sf::Vector2f delta) { 
		m_aabb.setPosition(m_aabb.getPosition() + delta);
		m_position += delta;
	}
	void move(float x, float y) { 
		//Logger::Log<Debug>(x, y);
		m_aabb.setPosition(m_aabb.getPosition() + sf::Vector2f(x, y));
		m_position += sf::Vector2f(x, y);
	}

protected:
	AABB m_aabb;
	BodyType m_type;
	BodyStatus m_status;

	sf::Vector2f m_position;

	virtual void onCollide(Body* other, const CollisionDetail& detail) {}
	virtual void afterCollisionTestCallback() {}

	bool m_freezed = 0;
};