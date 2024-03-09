#include "Characters/ToasterBot.hpp"

const CharacterConfig ToasterBot::s_config {
	1000.0f,		// health
	50.0f,		// attack
	0.4f,		// speed
	2000.0f,
	200.0f,

	4.0f,		// scale
	sf::Vector2f{16, 11},
	sf::Vector2f{16.0f, 15.0f}		// size
};

ToasterBot::ToasterBot(PhysicsWorld* world)
	: Character(s_config, world) {
	initAnimation();
}

void ToasterBot::initAnimation() {
	m_animation.addAnimation(
		Idle,
		Animation("Characters/Toaster Bot/idle.png", 106, 160, LeftRight)
	);
	m_animation.addAnimation(
		Walk,
		Animation("Characters/Toaster Bot/run.png", 106, 120, LeftRight)
	);
	m_animation.addAnimation(
		Attack,
		Animation("Characters/Toaster Bot/attack.png", 106, 80, LeftRight),
		[this]() { m_status.attacking = 0; }
	);
	m_animation.addAnimation(
		Hit,
		Animation("Characters/Toaster Bot/damaged.png", 106, 120, LeftRight),
		[this]() { m_status.hit = 0; }
	);
	m_animation.addAnimation(
		Death,
		Animation("Characters/Toaster Bot/death.png", 106, 120, LeftRight)
	);

	m_animation.setCurrentAnimation(Idle);
	m_animation.setUpdateFunction(std::bind(&ToasterBot::animationUpdate, this));
	m_animation.addKeyFrameCallback(Attack, 7, std::bind(&ToasterBot::attack, this));
	m_animation.addKeyFrameCallback(Attack, 8, std::bind(&ToasterBot::resetAttack, this));
}

void ToasterBot::myUpdate(float dt) {
	if (KeyState::getMouse(sf::Mouse::Left).pressed && !m_status.attacking) {
		m_status.attacking = 1;
	}
}

AnimationType ToasterBot::animationUpdate() {
	AnimationType ret = Idle;

	if (m_status.dead) {
		return None;
	}

	if (m_status.moving) {
		ret = Walk;
	}

	if (m_status.attacking) {
		ret = Attack;
	}

	if (m_status.hit) {
		ret = Hit;
	}

	if (m_status.health <= 0) {
		ret = Death;
		m_status.dead = 1;
	}

	return ret;
}

void ToasterBot::attack() {
	const AABB aabb = (m_direction == CharacterDirection::Right ? m_attackRangeR : m_attackRangeL) + m_position;

	if (!m_world) {
		return;
	}

	const std::vector<Body*> hit = m_world->query<BodyType::Enemy>(aabb);

	for (Body* body : hit) {
		if (m_hit[body]) {
			continue;
		}
		m_hit[body] = 1;
		body->getStatus().health -= m_config.attack;
		body->getStatus().hit = 1;
	}
}

void ToasterBot::resetAttack() {
	m_hit.clear();
}
