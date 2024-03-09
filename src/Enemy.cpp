#include "Enemy.hpp"

Enemy::Enemy(const EnemyConfig& config, PhysicsWorld* world)
	: m_direction(CharacterDirection::Right), m_config(config), m_world(world),
		Body(BodyType::Enemy,
			AABB(sf::Vector2f(
				m_position.x + config.scale * (config.symmetryAxis.x - config.size.x * 0.5f), 
				m_position.y + config.scale * (config.symmetryAxis.y - config.size.y * 0.5f)
			), sf::Vector2f(config.size.x* config.scale, config.size.y* config.scale))
		) {
	m_status.health = m_config.health;
}

void Enemy::update(float dt) {
	m_status.moving = 0;

	if (!m_status.dead && !m_status.shocked) {
		myUpdate(dt);
	}

	if (!m_status.moving) {
		m_status.velocity.x *= (m_status.onGround && !m_status.shocked) ? 0.7 : 0.99;
	}

	if (m_status.shocked) {
		m_status.shockDuration -= dt;
		if (m_status.shockDuration <= 0.0f) {
			m_status.shocked = 0;
			m_status.shockDuration = 0.0f;
		}
	}

	m_status.onWall = 0;
	m_status.onGround = 0;
	m_animation.update(dt);
	move(m_status.velocity * dt);
}

void Enemy::draw(sf::RenderTarget& window) {
	Frame frame = m_animation.getFrame();
	m_sprite.setPosition(m_position);
	m_sprite.setTexture(*frame.texture);
	m_sprite.setTextureRect(frame.rect);
	turnCharacter();
	m_sprite.setScale(m_config.scale, m_config.scale);
	drawHealthBar(window);
	window.draw(m_sprite);
}

void Enemy::turnCharacter() {
	if (m_direction == CharacterDirection::Right) {
		return;
	}

	sf::IntRect rect = m_sprite.getTextureRect();
	m_sprite.setPosition(m_sprite.getPosition() - sf::Vector2f(rect.width + -2 * m_config.symmetryAxis.x, 0) * m_config.scale);
	m_sprite.setTextureRect(sf::IntRect(rect.left + rect.width, rect.top, -rect.width, rect.height));
}

void Enemy::drawHealthBar(sf::RenderTarget& window) const {
	if (m_status.health <= 0) {
		return;
	}
	const sf::Vector2f size{ 50.0f, 10.0f };
	const float yoffset = 10.0f;
	sf::RectangleShape red({ size.x * (m_status.health / m_config.health), size.y });
	red.setPosition({
		m_position.x + m_config.symmetryAxis.x * m_config.scale - size.x * 0.5f,
		m_position.y + (m_config.symmetryAxis.y - m_config.size.y * 0.5f - yoffset) * m_config.scale});
	red.setFillColor(sf::Color::Red);
	sf::RectangleShape white({ size.x - red.getSize().x, size.y });
	white.setPosition({
		red.getPosition().x + red.getSize().x,
		red.getPosition().y });
	white.setFillColor(sf::Color::White);

	window.draw(red);
	window.draw(white);
}

void Enemy::onCollide(Body* other, const CollisionDetail& detail) {
	m_lastCollision = detail;
	if (other->getType() != BodyType::Static) {
		return;
	}

	myOnCollide(other, detail);
}

void Enemy::afterCollisionTestCallback() {

}

void Enemy::myUpdate(float dt) {

}

void Enemy::myOnCollide(Body* other, const CollisionDetail& detail) {

}
