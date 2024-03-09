#pragma once
#include <time.h>
#include <random>

#include "Character.hpp"

#include "Physics/PhysicsWorld.hpp"
#include "Physics/Body.hpp"
#include "Physics/AABB.hpp"

#include "Animation.hpp"

class Chest : public Body {
public:
	Chest();

	void update(float dt);

	void draw(sf::RenderTarget& window);

	bool getOpened() const { return m_opened; }
private:
	static const CharacterConfig s_config;

	AnimationManager m_animation;
	sf::Sprite m_sprite;

	int m_chestType;
	bool m_opened = 0;

	void initAnimation();

	bool m_openAnimationPlaying = 0;

	AnimationType animationUpdate();
	virtual void onCollide(Body* other, const CollisionDetail& detail) override;
	virtual void afterCollisionTestCallback() override;
};