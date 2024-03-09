#include "Physics/Line.hpp"

Line::Line() 
	: m_start(0.0f, 0.0f), m_end(0.0f, 0.0f) {
}

Line::Line(float x1, float y1, float x2, float y2)
	: m_start(x1, y1), m_end(x2, y2) {
}


Line::Line(const sf::Vector2f& p1, float x2, float y2)
	: m_start(p1), m_end(x2, y2) {
}


Line::Line(float x1, float y1, const sf::Vector2f& p2)
	: m_start(x1, y1), m_end(p2) {
}


Line::Line(const sf::Vector2f& p1, const sf::Vector2f& p2)
	: m_start(p1), m_end(p2) {
}


bool Line::operator== (const Line& other) const {
	return m_start == other.m_start && m_end == other.m_end;
}
bool Line::operator!= (const Line& other) const {
	return m_start != other.m_start || m_end != other.m_end;
}

Line Line::operator+(const sf::Vector2f& delta) const {
	return { m_start + delta, m_end + delta };
}
Line& Line::operator+=(const sf::Vector2f& delta) {
	m_start += delta;
	m_end += delta;
	return *this;
}
Line Line::operator-(const sf::Vector2f& delta) const {
	return { m_start - delta, m_end - delta };
}
Line& Line::operator-=(const sf::Vector2f& delta) {
	m_start -= delta;
	m_end -= delta;
	return *this;
}

bool Line::intersect(const AABB& other) const {
	const float a = m_end.y - m_start.y;
	const float b = m_start.x - m_end.x;
	const float c = m_end.x * m_start.y - m_start.x * m_end.y;

	const float tl = other.left() * a + other.top() * b + c;
	const float tr = other.right() * a + other.top() * b + c;
	const float br = other.right() * a + other.bottom() * b + c;
	const float bl = other.left() * a + other.bottom() * b + c;
	if (tl > 0 && tr > 0 && br > 0 && bl > 0
		|| tl < 0 && tr < 0 && br < 0 && bl < 0) {
		return false;
	}

	const sf::Vector2f pos{ std::min(m_start.x, m_end.x), std::min(m_start.y, m_end.y) };
	const sf::Vector2f size{ std::abs(m_start.x - m_end.x), std::abs(m_start.y - m_end.y) };
	return AABB(pos, size).intersect(other);
}

void Line::draw(sf::RenderTarget& window) const {
	sf::VertexArray line(sf::LinesStrip, 2);
	line[0].position = m_start;
	line[0].color = sf::Color::Black;
	line[1].position = m_end;
	line[1].color = sf::Color::Black;
	
	window.draw(line);
}