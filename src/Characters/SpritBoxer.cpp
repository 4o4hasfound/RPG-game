#include "Characters/SpritBoxer.hpp"


const CharacterConfig SpritBoxer::s_config{
	500.0f,		// health
	40.0f,		// attack
	0.4f,		// speed
	2000.0f,
	200.0f,

	3.0f,		// scale
	sf::Vector2f{16, 27},
	sf::Vector2f{13.0f, 18.0f}		// size
};

SpritBoxer::SpritBoxer(PhysicsWorld* world)
	: Character(s_config, world), m_timeToLastAtk(0.0f), m_nAtk(0), m_dashCoolDown(0.0f), m_trembleCounter(0.0f) {
	initAnimation();
}

void SpritBoxer::initAnimation() {
	m_animation.addAnimation(
		Idle,
		Animation("Characters/Spirit Boxer/idle.png", 44, 160, TopDown)
	);
	m_animation.addAnimation(
		Walk,
		Animation("Characters/Spirit Boxer/run.png", 44, 120, TopDown)
	);
	m_animation.addAnimation(
		Attack1,
		Animation("Characters/Spirit Boxer/attack 1.png", 44, 80, TopDown),
		[this]() { m_nAtk = 1; m_status.attacking = 0; }
	);
	m_animation.addAnimation(
		Attack2,
		Animation("Characters/Spirit Boxer/attack 2.png", 44, 80, TopDown),
		[this]() { m_nAtk = 3; m_status.attacking = 0;  }
	);
	m_animation.addAnimation(
		Attack3,
		Animation("Characters/Spirit Boxer/attack 3.png", 44, 80, TopDown),
		[this]() { m_nAtk = 0; m_status.attacking = 0;  }
	);
	m_animation.addAnimation(
		Hit,
		Animation("Characters/Spirit Boxer/Damaged & Death.png", 44, 120, TopDown),
		[this]() { 
			m_status.hit = 0;
			m_nAtk = 0;
			m_status.attacking = 0;
		}
	);
	m_animation.getAnimation(Hit).setEndFrame(3);
	m_animation.addAnimation(
		Death,
		Animation("Characters/Spirit Boxer/Damaged & Death.png", 44, 120, TopDown),
		[this]() {
			deadAnimationPlaying = 0;
		}
	);
	m_animation.getAnimation(Death).setStartFrame(3);

	m_animation.setCurrentAnimation(Idle);
	m_animation.setUpdateFunction(std::bind(&SpritBoxer::animationUpdate, this));
	m_animation.addKeyFrameCallback(Attack1, 2, std::bind(&SpritBoxer::attack, this));
	m_animation.addKeyFrameCallback(Attack1, 3, std::bind(&SpritBoxer::resetAttack, this));

	m_animation.addKeyFrameCallback(Attack1, 5, std::bind(&SpritBoxer::attack, this));
	m_animation.addKeyFrameCallback(Attack1, 6, std::bind(&SpritBoxer::resetAttack, this));

	m_animation.addKeyFrameCallback(Attack2, 5, std::bind(&SpritBoxer::attack, this));
	m_animation.addKeyFrameCallback(Attack2, 6, std::bind(&SpritBoxer::resetAttack, this));

	m_animation.addKeyFrameCallback(Attack2, 9, std::bind(&SpritBoxer::attack, this));
	m_animation.addKeyFrameCallback(Attack2, 10, std::bind(&SpritBoxer::resetAttack, this));

	m_animation.addKeyFrameCallback(Attack3, 4, std::bind(&SpritBoxer::attack, this));
	m_animation.addKeyFrameCallback(Attack3, 5, std::bind(&SpritBoxer::attack, this));
	m_animation.addKeyFrameCallback(Attack3, 6, std::bind(&SpritBoxer::resetAttack, this));
}

void SpritBoxer::myUpdate(float dt) {
	if (KeyState::getMouse(sf::Mouse::Right).pressed &&
		!KeyState::getLastMouse(sf::Mouse::Right).held && 
		m_status.skillCooldown <= 0.0f && !m_status.inSkill ) {
		m_status.inSkill = 1;
		m_status.skillDuration = m_config.skillDuration;
		m_status.skillCooldown = m_config.skillCooldown;
		m_status.velocity.x = (m_direction == CharacterDirection::Right ? 1.0 : -1.0) * 2.0f;
	}
	if (m_status.skillDuration <= 0.0f) {
		m_status.inSkill = 0;
		if (m_trembleCounter == 0) windowTremble = 0;
		if (m_direction == CharacterDirection::Right) {
			m_status.velocity.x = std::min(0.5f, m_status.velocity.x);
		}
		else {
			m_status.velocity.x = std::max(-0.5f, m_status.velocity.x);
		}
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
	if (m_timeToLastAtk >= 500) {
		m_nAtk = 0;
		m_timeToLastAtk = 0;
	}
	if (m_trembleCounter > 0) {
		m_trembleCounter -= dt;
		if (m_trembleCounter == 0) {
			m_trembleCounter -= 1.0f;
		}
	}
	else if (m_trembleCounter < 0) {
		windowTremble = 0;
		m_trembleCounter = 0;
	}
}

void SpritBoxer::myOnCollide(Body* other, const CollisionDetail& detail) {
	if (other->getType() != BodyType::Enemy) {
		return;
	}

	if (m_status.inSkill && !other->getStatus().dead) {
		windowTremble = 20.0f;
		other->getStatus().health -= m_config.attack * 0.5f;
		if (other->getStatus().health <= 0) {
			m_status.skillCooldown = 0.0f;
		}
		other->getStatus().shocked = 1;
		other->getStatus().shockDuration = 1000.0f;
		other->getStatus().velocity.x += (m_direction == CharacterDirection::Right ? 1.0f : -1.0f) * 1.0f;
		other->getStatus().velocity.y -= 0.5f;

		m_status.skillDuration = 0.0f;
		m_status.velocity.x = 0.0f;
	}
}

AnimationType SpritBoxer::animationUpdate() {
	AnimationType ret = Idle;

	if (m_status.dead) {
		return None;
	}

	if (m_status.moving) {
		ret = Walk;
	}

	if (m_status.attacking) {
		switch (m_nAtk) {
		case 0:
			ret = Attack1;
			break;
		case 1:
		case 2:
			ret = Attack2;
			break;
		case 3:
			ret = Attack3;
			break;
		}
	}

	if (m_status.hit) {
		windowTremble = 1;
		m_trembleCounter = 5.0f;
		ret = Hit;
	}

	if (m_status.health <= 0) {
		ret = Death;
		windowTremble = 1;
		m_trembleCounter = 250.0f;
		m_status.dead = 1;
		deadAnimationPlaying = 1;
	}

	return ret;
}

void SpritBoxer::attack() {
	const AABB aabb = (m_direction == CharacterDirection::Right ? m_attackRangeR : m_attackRangeL)[m_nAtk] + m_position;
	if (window) {
		//(aabb).draw(*window);
	}
	if (!m_world) {
		return;
	}

	const std::vector<Body*> hit = m_world->query<BodyType::Enemy>(aabb);
	if (hit.size()) {
		windowTremble = 3;
	}

	for (Body* body : hit) {
		if (m_hit[body]) {
			continue;
		}
		m_hit[body] = 1;
		float atk = m_config.attack;
		switch (m_nAtk) {
		case 0:
			atk *= 0.25f;
			break;
		case 1:
		case 2:
			atk *= 0.5f;
			break;
		}
		body->getStatus().health -= m_config.attack;
		body->getStatus().hit = 1;
	}
}

void SpritBoxer::resetAttack() {
	if (m_trembleCounter == 0) windowTremble = 0;
	if (m_nAtk == 1) {
		m_nAtk = 2;
	}
	m_hit.clear();
}
