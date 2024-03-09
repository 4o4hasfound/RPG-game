#pragma once
#include <SFML/Graphics.hpp>

#include "Physics/AABB.hpp"

class Line {
public:
	Line();
	Line(float x1, float y1, float x2, float y2);
	Line(const sf::Vector2f& p1, float x2, float y2);
	Line(float x1, float y1, const sf::Vector2f& p2);
	Line(const sf::Vector2f& p1, const sf::Vector2f& p2);

	bool operator== (const Line& other) const;
	bool operator!= (const Line& other) const;

	Line operator+(const sf::Vector2f& delta) const;
	Line& operator+=(const sf::Vector2f& delta);
	Line operator-(const sf::Vector2f& delta) const;
	Line& operator-=(const sf::Vector2f& delta);

	bool intersect(const AABB& other) const;

	const sf::Vector2f& getStart() const { return m_start; }
	void setStart(const sf::Vector2f& position) { m_start = position; }

	const sf::Vector2f& getEnd() const { return m_end; }
	void setEnd(const sf::Vector2f& position) { m_end = position; }

	float getMagnitude() const { return std::sqrt(std::pow(m_start.x - m_end.x, 2) + std::pow(m_start.y - m_end.y, 2)); }

	void draw(sf::RenderTarget& window) const;
private:
	sf::Vector2f m_start;
	sf::Vector2f m_end;
};