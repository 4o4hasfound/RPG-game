#pragma once

#include "SFML/Graphics.hpp"
#include "tmxlite/Types.hpp"

class AABB {
public:
	AABB();
	AABB(float x, float y, float width, float height);
	AABB(const sf::Vector2f& position, float width, float height);
	AABB(const sf::Vector2f& position, const sf::Vector2f& size);
	AABB(const tmx::FloatRect& rect);

	bool operator== (const AABB& other) const;
	bool operator!= (const AABB& other) const;

	AABB operator+(const sf::Vector2f& delta) const;
	AABB& operator+=(const sf::Vector2f& delta);
	AABB operator-(const sf::Vector2f& delta) const;
	AABB& operator-=(const sf::Vector2f& delta);

	bool intersect(const AABB& other) const;
	bool contain(const AABB& other) const;
	AABB merge(const AABB& other);

	const sf::Vector2f& size() const { return m_size; }
	float width() const { return m_size.x; }
	float height() const { return m_size.y; }

	void setSize(const sf::Vector2f& size) { m_size = size; }
	void setWidth(float width) { m_size.x = width; }
	void setHeight(float height) { m_size.y= height; }

	const sf::Vector2f& getPosition() const { return m_position; }
	void setPosition(const sf::Vector2f& position) { m_position = position; }
	void setLeft(float x) { 
		m_size.x += m_position.x - x;
		m_position.x = x; 
	}
	void setTop(float y) {
		m_size.y += m_position.y - y;
		m_position.y = y; 
	}
	void setRight(float x) { m_size.x = x - m_position.x; }
	void setBottom(float y) { m_size.y = y - m_position.y; }

	float right() const;
	float left() const;
	float bottom() const;
	float top() const;

	void draw(sf::RenderTarget& window) const;
private:
	sf::Vector2f m_position;
	sf::Vector2f m_size;
};