#include "Physics/Circle.hpp"

Circle::Circle()
	: m_position(0.0f, 0.0f), m_radius(0.0f) {
}

Circle::Circle(float x, float y, float radius) 
	: m_position(x, y), m_radius(radius) {

}

Circle::Circle(const sf::Vector2f& position, float radius)
	: m_position(position), m_radius(radius) {

}

bool Circle::operator== (const Circle& other) const {
	return m_position == other.m_position && m_radius == other.m_radius;
}
bool Circle::operator!= (const Circle& other) const {
	return m_position != other.m_position || m_radius != other.m_radius;
}

Circle Circle::operator+(const sf::Vector2f& delta) const {
	return { m_position + delta, m_radius };
}
Circle& Circle::operator+=(const sf::Vector2f& delta) {
	m_position += delta;
	return *this;
}
Circle Circle::operator-(const sf::Vector2f& delta) const {
	return { m_position - delta, m_radius };
}
Circle& Circle::operator-=(const sf::Vector2f& delta) {
	m_position -= delta;
	return *this;
}

bool Circle::intersect(const AABB& other) const { 
	float testX = m_position.x;
	float testY = m_position.y;

	if (m_position.x < other.left())			testX = other.left();
	else if (m_position.x > other.right())		testX = other.right();
	if (m_position.y < other.top())				testY = other.top();
	else if (m_position.y > other.bottom())		testY = other.bottom();

	float distX = m_position.x - testX;
	float distY = m_position.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	return distance <= m_radius;
}

void Circle::draw(sf::RenderTarget& window) const {
	sf::CircleShape circle(m_radius);
	circle.setPosition(m_position - sf::Vector2f{m_radius, m_radius});
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(5);
	circle.setOutlineColor(sf::Color::Black);
	window.draw(circle);
}
