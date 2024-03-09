#pragma once
#include <SFML/Graphics.hpp>

#include "Physics/AABB.hpp"

class Circle {
public:
	Circle();
	Circle(float x, float y, float radius);
	Circle(const sf::Vector2f& position, float radius);

	bool operator== (const Circle& other) const;
	bool operator!= (const Circle& other) const;

	Circle operator+(const sf::Vector2f& delta) const;
	Circle& operator+=(const sf::Vector2f& delta);
	Circle operator-(const sf::Vector2f& delta) const;
	Circle& operator-=(const sf::Vector2f& delta);

	bool intersect(const AABB& other) const;

	float getRadius() const { return m_radius; }
	float setRadius(float radius) { m_radius = radius; }

	const sf::Vector2f& getPosition() const { return m_position; }
	void setPosition(const sf::Vector2f& position) { m_position = position; }

	void draw(sf::RenderTarget& window) const;
private:
	sf::Vector2f m_position;
	float m_radius;
};