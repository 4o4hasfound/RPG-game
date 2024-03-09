#include "Physics/AABB.hpp"

AABB::AABB() 
	: m_position(0.0f, 0.0f), m_size(0.0f, 0.0f) {
}

AABB::AABB(float x, float y, float width, float height)
	: m_position(x, y), m_size(width, height) {
}

AABB::AABB(const sf::Vector2f& position, float width, float height) 
	: m_position(position), m_size(width, height) {
}

AABB::AABB(const sf::Vector2f& position, const sf::Vector2f& size)
	: m_position(position), m_size(size) {
}

AABB::AABB(const tmx::FloatRect& rect) 
	: m_position(rect.left, rect.top), m_size(rect.width, rect.height) {

}

bool AABB::operator== (const AABB& other) const {
	return m_position == other.m_position && m_size == other.m_size;
}
bool AABB::operator!= (const AABB& other) const {
	return m_position != other.m_position || m_size != other.m_size;
}

AABB AABB::operator+(const sf::Vector2f& delta) const {
	return { m_position + delta, m_size };
}
AABB& AABB::operator+=(const sf::Vector2f& delta) {
	m_position += delta;
	return *this;
}
AABB AABB::operator-(const sf::Vector2f& delta) const {
	return { m_position - delta, m_size };
}
AABB& AABB::operator-=(const sf::Vector2f& delta) {
	m_position -= delta;
	return *this;
}

bool AABB::intersect(const AABB& other) const {
	return right() > other.left() && other.right() > left() &&
		bottom() > other.top() && other.bottom() > top();
}

bool AABB::contain(const AABB& other) const {
	return right() > other.right() && left() < other.left() && top() < other.top() && bottom() > other.bottom();
}

AABB AABB::merge(const AABB& other) {
	if (!width() || !height()) {
		return other;
	}

	const sf::Vector2f topleft{
		std::min(left(), other.left()),
		std::min(top(), other.top())
	};
	const sf::Vector2f bottomright{
		std::max(right(), other.right()),
		std::max(bottom(), other.bottom())
	};
	
	return AABB(topleft, bottomright - topleft);
}

float AABB::right() const {
	return m_position.x + m_size.x;
}

float AABB::left() const {
	return m_position.x;
}

float AABB::bottom() const {
	return m_position.y + m_size.y;
}

float AABB::top() const {
	return m_position.y;
}

void AABB::draw(sf::RenderTarget& window) const {
	sf::RectangleShape rect;
	rect.setPosition(m_position);
	rect.setSize(m_size);
	rect.setOutlineColor(sf::Color::Black);
	rect.setOutlineThickness(10);
	rect.setFillColor(sf::Color::Transparent);
	window.draw(rect);
}
