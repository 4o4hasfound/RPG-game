#pragma once

#include "Physics/Body.hpp"
#include "Physics/AABB.hpp"
#include "Character.hpp"

class Thorn : public Body {
public:
	Thorn(const AABB& aabb);
private:
	virtual void onCollide(Body* other, const CollisionDetail& detail) override final;
	virtual void afterCollisionTestCallback() override final;
};