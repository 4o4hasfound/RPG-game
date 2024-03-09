#include "Objects/Heal.hpp"

Heal::Heal(const AABB& aabb)
	: Body(BodyType::Static_Uncollidable, aabb) {

}

void Heal::onCollide(Body* other, const CollisionDetail& detail) {
	if (other->getType() == BodyType::Character) {
		if (other->getStatus().health + 1 <= dynamic_cast<Character*>(other)->getConfig().health) {
			other->getStatus().health += 1;
		}
	}
}

void Heal::afterCollisionTestCallback() {

}
