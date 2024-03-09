#include "Characters/BACBot.hpp"


const CharacterConfig BACBot::s_config{
	500.0f,		// health
	40.0f,		// attack
	0.4f,		// speed
	2000.0f,
	200.0f,

	4.0f,		// scale
	sf::Vector2f{66, 19},
	sf::Vector2f{21.0f, 24.0f}		// size
};

BACBot::BACBot(PhysicsWorld* world)
	: Character(s_config, world), m_timeToLastAtk(0.0f), m_nAtk(0), m_dashCoolDown(0.0f) {
	initAnimation();
}

void BACBot::initAnimation() {
	m_animation.addAnimation(
		Idle,
		Animation("Characters/Ball and Chain Bot/idle.png", 39, 160, TopDown)
	);
	m_animation.addAnimation(
		Walk,
		Animation("Characters/Ball and Chain Bot/run.png", 39, 120, TopDown)
	);
	m_animation.addAnimation(
		Attack,
		Animation("Characters/Ball and Chain Bot/attack.png", 39, 80, TopDown),
		[this]() { m_status.attacking = 0; }
	);
	m_animation.addAnimation(
		Hit,
		Animation("Characters/Ball and Chain Bot/hit.png", 39, 120, TopDown),
		[this]() {
			m_status.hit = 0;
			m_status.attacking = 0;
		}
	);
	m_animation.addAnimation(
		Death,
		Animation("Characters/Ball and Chain Bot/death.png", 39, 120, TopDown)
	);

	m_animation.setCurrentAnimation(Idle);
	m_animation.setUpdateFunction(std::bind(&BACBot::animationUpdate, this));
	m_animation.addKeyFrameCallback(Attack, 1, [this]() {m_nAtk = 0; attack(); });
	m_animation.addKeyFrameCallback(Attack, 2, [this]() {m_nAtk = 1; resetAttack(); });

	m_animation.addKeyFrameCallback(Attack, 5, [this]() { m_nAtk = 1; attack(); });
	m_animation.addKeyFrameCallback(Attack, 6, [this]() { m_nAtk = 0; resetAttack(); });
}

void BACBot::myUpdate(float dt) {
	if (KeyState::getMouse(sf::Mouse::Right).pressed && m_status.skillCooldown <= 0.0f && !m_status.inSkill) {
		m_status.inSkill = 1;
		m_status.skillDuration = m_config.skillDuration;
		m_status.skillCooldown = m_config.skillCooldown;
		m_status.velocity.x += (m_direction == CharacterDirection::Right ? 1.0 : -1.0) * 5.0f;
	}
	if (m_status.skillDuration <= 0.0f) {
		m_status.inSkill = 0;
		m_status.velocity.x = std::min(0.5f, m_status.velocity.x);
	}
	if (m_status.inSkill) {
		m_status.skillDuration -= dt;
	}
	else {
		m_status.skillCooldown -= dt;
	}
	if (KeyState::getMouse(sf::Mouse::Left).pressed && !m_status.attacking) {
		m_timeToLastAtk = 0;
		m_status.attacking = 1;
	}
	else {
		if (!m_status.attacking) {
			m_timeToLastAtk += dt;
		}
	}
	if (m_timeToLastAtk >= 1000) {
		m_nAtk = 0;
		m_timeToLastAtk = 0;
	}
}

void BACBot::myOnCollide(Body* other, const CollisionDetail& detail) {
	if (other->getType() != BodyType::Enemy) {
		return;
	}

	if (m_status.inSkill) {
		other->getStatus().health -= m_config.attack;
		other->getStatus().shocked = 1;
		other->getStatus().shockDuration = 1500.0f;
		other->getStatus().velocity.x += (m_direction == CharacterDirection::Right ? 1.0f : -1.0f) * 3.0f;
		other->getStatus().velocity.y -= 0.5f;

		m_status.skillDuration = 0.0f;
		m_status.velocity.x = 0.0f;
	}
}

AnimationType BACBot::animationUpdate() {
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

void BACBot::attack() {
	const AABB aabb = (m_direction == CharacterDirection::Right ? m_attackRangeR : m_attackRangeL)[m_nAtk] + m_position;
	if (window) {
		//(aabb).draw(*window);
	}
	if (!m_world) {
		return;
	}

	const std::vector<Body*> hit = m_world->query<BodyType::Enemy>(aabb);

	for (Body* body : hit) {
		if (m_hit[body]) {
			continue;
		}
		m_hit[body] = 1;
		float atk = m_config.attack;
		body->getStatus().health -= m_config.attack;
		body->getStatus().hit = 1;
	}
}

void BACBot::resetAttack() {
	m_hit.clear();
}
