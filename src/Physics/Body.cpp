#include "Physics/Body.hpp"

Body::Body(BodyType type, AABB aabb)
	: m_type(type), m_aabb(aabb) {

}

Body::~Body() {

}
