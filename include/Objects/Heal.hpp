#pragma once

#include "Physics/Body.hpp"
#include "Physics/AABB.hpp"
#include "Character.hpp"

class Heal : public Body {
public:
	Heal(const AABB& aabb);
private:
	virtual void onCollide(Body* other, const CollisionDetail& detail) override final;
	virtual void afterCollisionTestCallback() override final;
};