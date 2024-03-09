#include "Character.hpp"

Character::Character(const CharacterConfig& config, PhysicsWorld* world)
	: m_direction(CharacterDirection::Right), m_config(config),  m_world(world),
		Body(BodyType::Character,
			AABB(sf::Vector2f(
				m_position.x + config.scale * (config.symmetryAxis.x - config.size.x * 0.5f),
				m_position.y + config.scale * (config.symmetryAxis.y - config.size.y * 0.5f)
			), sf::Vector2f(config.size.x* config.scale, config.size.y* config.scale))
		) {	
	m_status.health = m_config.health;
}

void Character::update(float dt) {
	m_status.moving = 0;

	// W pressed
	if (KeyState::getKey(sf::Keyboard::W).held && m_status.onGround && !m_status.dead) {
		m_status.moving = 1;
		m_status.velocity.y = -1.5f;

		if (m_status.onWall) {
			m_status.velocity.x = 0;
			if (m_lastCollision.direction.x == 1 || m_lastCollision.direction.x == -1) {
				m_status.velocity.x = m_lastCollision.direction.x * 0.6f;
			}
		}
	}

	// A pressed
	if (KeyState::getKey(sf::Keyboard::A).held && !m_status.inSkill && !m_status.dead) {
		m_status.moving = 1;
		m_direction = Left;

		if (m_status.onGround) {
			float toMove = m_status.velocity.x < -0.5f ? 0.0f : 0.004f;
			m_status.velocity.x -= toMove * dt;
		}
		else {
			float toMove = m_status.velocity.x < -0.5f ? 0.0f : 0.002f;
			m_status.velocity.x -= toMove * dt;
		}
	}
	// D pressed
	else if (KeyState::getKey(sf::Keyboard::D).held && !m_status.inSkill && !m_status.dead) {
		m_status.moving = 1;
		m_direction = Right;

		if (m_status.onGround) {
			float toMove = m_status.velocity.x > 0.5f ? 0.0f: 0.004f;
			m_status.velocity.x += toMove * dt;
		}
		else {
			float toMove = m_status.velocity.x > 0.5f ? 0.0f : 0.002f;
			m_status.velocity.x += toMove * dt;
		}
	}

	myUpdate(dt);

	if (!m_status.moving) {
		m_status.velocity.x *= ((m_status.onGround && !m_status.inSkill) ? 0.7: 0.99);
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

void Character::draw(sf::RenderTarget& window) {
	Frame frame = m_animation.getFrame();
	m_sprite.setPosition(m_position);
	m_sprite.setTexture(*frame.texture);
	m_sprite.setTextureRect(frame.rect);
	turnCharacter();
	m_sprite.setScale(m_config.scale, m_config.scale);
	drawHealthBar(window);
	drawSkillBar(window);
	window.draw(m_sprite);
}

void Character::turnCharacter() {
	if (m_direction == Right) {
		return;
	}

	sf::IntRect rect = m_sprite.getTextureRect();
	m_sprite.setPosition(m_sprite.getPosition() - sf::Vector2f(rect.width + -2 * m_config.symmetryAxis.x, 0) * m_config.scale);
	m_sprite.setTextureRect(sf::IntRect(rect.left + rect.width, rect.top, -rect.width, rect.height));
}

void Character::drawHealthBar(sf::RenderTarget& window) const {
	if (m_status.health <= 0) {
		return;
	}
	const sf::Vector2f size{ 50.0f, 10.0f };
	const float yoffset = 5.0f;
	sf::RectangleShape red({ size.x * (m_status.health / m_config.health), size.y });
	red.setPosition({
		m_position.x + m_config.symmetryAxis.x * m_config.scale - size.x / 2.0f,
		m_position.y + m_config.symmetryAxis.y - (m_config.size.y * 0.5f + yoffset) * m_config.scale });
	red.setFillColor(sf::Color::Red);
	sf::RectangleShape white({ size.x - red.getSize().x, size.y });
	white.setPosition({
		red.getPosition().x + red.getSize().x,
		red.getPosition().y });
	white.setFillColor(sf::Color::White);

	window.draw(red);
	window.draw(white);
}

void Character::drawSkillBar(sf::RenderTarget& window) const {
	if (m_status.inSkill) {
		const sf::Vector2f size{ 50.0f, 10.0f };
		const float yoffset = 1.0f;
		sf::RectangleShape magenta({ size.x * (1 - (m_status.skillDuration / m_config.skillDuration)), size.y });
		magenta.setPosition({
			m_position.x + m_config.symmetryAxis.x * m_config.scale - size.x / 2.0f,
			m_position.y + m_config.symmetryAxis.y - (m_config.size.y * 0.5f + yoffset) * m_config.scale });
		magenta.setFillColor(sf::Color::Magenta);
		sf::RectangleShape white({ size.x - magenta.getSize().x, size.y });
		white.setPosition({
			magenta.getPosition().x + magenta.getSize().x,
			magenta.getPosition().y });
		white.setFillColor(sf::Color::White);

		window.draw(magenta);
		window.draw(white);
		return;
	}
	const sf::Vector2f size{ 50.0f, 10.0f };
	const float yoffset = 1.0f;
	sf::RectangleShape green({ size.x * (1 - std::max(0.0f, m_status.skillCooldown / m_config.skillCooldown)), size.y });
	green.setPosition({
		m_position.x + m_config.symmetryAxis.x * m_config.scale - size.x / 2.0f,
		m_position.y - yoffset * m_config.scale });
	green.setFillColor(sf::Color::Green);
	sf::RectangleShape white({ size.x - green.getSize().x, size.y });
	white.setPosition({
		green.getPosition().x + green.getSize().x,
		green.getPosition().y });
	white.setFillColor(sf::Color::White);

	window.draw(green);
	window.draw(white);
}

void Character::onCollide(Body* other, const CollisionDetail& detail) {
	m_lastCollision = detail;
	myOnCollide(other, detail);
	if (other->getType() != BodyType::Static) {
		return;
	}

	if (detail.direction.x == 1 || detail.direction.x == -1) {
		m_status.onGround = 1;
		m_status.onWall = 1;
	}
}

void Character::afterCollisionTestCallback() {
}

void Character::myOnCollide(Body* other, const CollisionDetail& detail) {

}

void Character::myUpdate(float dt) {

}
