#include "Objects/Thorn.hpp"

Thorn::Thorn(const AABB& aabb)
	: Body(BodyType::Static, aabb) {

}

void Thorn::onCollide(Body* other, const CollisionDetail& detail) {
	other->getStatus().health = 0.0f;
	if (other->getType() == BodyType::Character && !other->getStatus().dead) {
		dynamic_cast<Character*>(other)->windowTremble = 5.0f;
	}
}

void Thorn::afterCollisionTestCallback() {

}
