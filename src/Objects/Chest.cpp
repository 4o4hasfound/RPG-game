#include "Objects/Chest.hpp"
const CharacterConfig Chest::s_config{
	0.0f,		// health
	0.0f,		// attack
	0.0f,		// speed
	0.0f,
	0.0f,

	2.5f,		// scale
	sf::Vector2f{ 16, 22 },
	sf::Vector2f{ 28.0f, 20.0f }		// size
};

Chest::Chest()
	: Body(BodyType::Static_Uncollidable,
		AABB(sf::Vector2f(
		m_position.x + s_config.scale * (s_config.symmetryAxis.x - s_config.size.x * 0.5f),
		m_position.y + s_config.scale * (s_config.symmetryAxis.y - s_config.size.y * 0.5f)
	), sf::Vector2f(s_config.size.x* s_config.scale, s_config.size.y* s_config.scale))) {
	m_chestType = rand() % 4;
	initAnimation();
}

void Chest::update(float dt) {
	if (m_opened && !m_openAnimationPlaying) {
		return;
	}
	m_animation.update(dt);
}

void Chest::draw(sf::RenderTarget& window) {
	if (m_opened && !m_openAnimationPlaying) {
		return;
	}
	Frame frame = m_animation.getFrame();
	m_sprite.setPosition(m_position);
	m_sprite.setTexture(*frame.texture);
	m_sprite.setTextureRect(frame.rect);
	m_sprite.setScale(s_config.scale, s_config.scale);
	window.draw(m_sprite);
}

void Chest::initAnimation() {
	m_animation.addAnimation(
		AnimationType::Idle,
		Animation("Chests/Chests-" + std::to_string(m_chestType * 2 + 1) + ".png", 48, 80.0f, AnimationDirection::LeftRight)
	);
	m_animation.addAnimation(
		AnimationType::Death,
		Animation("Chests/Chests-" + std::to_string(m_chestType * 2 + 2) + ".png", 48, 80.0f, AnimationDirection::LeftRight),
		[this]() {
			m_openAnimationPlaying = 0;
		}
	);
	m_animation.setUpdateFunction(std::bind(&Chest::animationUpdate, this));
	m_animation.setCurrentAnimation(AnimationType::Idle);
}

AnimationType Chest::animationUpdate() {
	if (m_opened) {
		return AnimationType::Death;
	}
	return AnimationType::Idle;
}

void Chest::onCollide(Body* other, const CollisionDetail& detail) {
	if (other->getType() != BodyType::Character) {
		return;
	}
	if (!m_opened) {
		m_opened = 1;
		m_openAnimationPlaying = 1;
	}
}

void Chest::afterCollisionTestCallback() {
}
