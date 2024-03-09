#include "Enemies/BigMudGuard.hpp"

const EnemyConfig BigMudGuard::s_config{
	100.0f,		// health
	50.0f,		// attack
	0.2f,		// speed

	10.0f,		// scale
	sf::Vector2f{19, 12},
	sf::Vector2f{15.0f, 16.0f}		// size
};

BigMudGuard::BigMudGuard(PhysicsWorld* world)
	: Enemy(s_config, world) {
	initAnimation();
}

void BigMudGuard::initAnimation() {
	m_animation.addAnimation(
		Idle,
		Animation("Characters/Mud Guard/idle.png", 23, 160, TopDown)
	);
	m_animation.addAnimation(
		Walk,
		Animation("Characters/Mud Guard/Run.png", 23, 120, TopDown)
	);
	m_animation.addAnimation(
		Attack1,
		Animation("Characters/Mud Guard/attack 1.png", 23, 40, TopDown),
		[this]() { m_nAtk = 1; m_status.attacking = 0; }
	);
	m_animation.addAnimation(
		Attack2,
		Animation("Characters/Mud Guard/attack 2.png", 23, 40, TopDown),
		[this]() { m_nAtk = 0; m_status.attacking = 0; }
	);
	m_animation.addAnimation(
		Hit,
		Animation("Characters/Mud Guard/damaged and death.png", 23, 120, TopDown),
		[this]() { m_status.hit = 0; }
	);
	m_animation.getAnimation(Hit).setEndFrame(3);
	m_animation.addAnimation(
		Death,
		Animation("Characters/Mud Guard/damaged and death.png", 23, 120, TopDown),
		[this]() { m_status.attacking = 0; }
	);
	m_animation.getAnimation(Death).setStartFrame(3);

	m_animation.setCurrentAnimation(Idle);
	m_animation.setUpdateFunction(std::bind(&BigMudGuard::animationUpdate, this));
	m_animation.addKeyFrameCallback(Attack1, 4, std::bind(&BigMudGuard::attack, this));
	m_animation.addKeyFrameCallback(Attack1, 5, std::bind(&BigMudGuard::resetAttack, this));

	m_animation.addKeyFrameCallback(Attack2, 4, std::bind(&BigMudGuard::attack, this));
	m_animation.addKeyFrameCallback(Attack2, 5, std::bind(&BigMudGuard::resetAttack, this));
}

void BigMudGuard::myUpdate(float dt) {
	const std::vector<Body*> query = m_world->query<BodyType::Character>(m_playerDetect + m_position);

	if (!query.size()) {
		return;
	}

	Body* target = nullptr;

	for (Body* body : query) {
		Character* character = dynamic_cast<Character*>(body);
		Line tmp{
			m_position + m_config.symmetryAxis * m_config.scale,
			character->getPosition() + character->getConfig().symmetryAxis * character->getConfig().scale };
		if (!m_world->query<BodyType::Static>(tmp).size()) {
			m_line = tmp;
			target = body;
			break;
		}
	}
	if (target == nullptr) {
		return;
	}
	if (target->getStatus().dead) {
		return;
	}

	// Move away from player
	if (std::abs(m_line.getStart().x - m_line.getEnd().x) < m_attackRangeL[m_nAtk].width() * 0.25f) {
		// Player is at the left, move right 
		if (target->getAABB().getPosition().x < m_aabb.getPosition().x &&
			!m_status.attacking &&
			m_world->query<BodyType::Static>(m_feetR + m_position).size()) {
			m_status.moving = 1;
			m_direction = Right;

			if (m_status.onGround) {
				m_status.velocity.x = std::min(m_status.velocity.x + 0.004f, 0.5f);
			}
			else {
				m_status.velocity.x = std::min(m_status.velocity.x + 0.002f, 0.5f);
			}
		}
		// Move Right
		else if (target->getAABB().getPosition().x > m_aabb.getPosition().x &&
			!m_status.attacking &&
			m_world->query<BodyType::Static>(m_feetL + m_position).size()) {
			m_status.moving = 1;
			m_direction = Left;

			if (m_status.onGround) {
				m_status.velocity.x = std::max(m_status.velocity.x - 0.004f, -0.5f);
			}
			else {
				m_status.velocity.x = std::max(m_status.velocity.x - 0.002f, -0.5f);
			}
		}

		if (m_status.moving) {
			// Jump
			AABB jumpDetect = (m_direction == CharacterDirection::Left ? m_left : m_right);
			if (m_world->query<BodyType::Static>(jumpDetect + m_position).size() && m_status.onGround && !m_status.dead) {
				m_status.moving = 1;
				m_status.velocity.y = -1.5f;

				if (m_status.onWall) {
					m_status.velocity.x = 0;
					if (m_lastCollision.direction.x == 1 || m_lastCollision.direction.x == -1) {
						m_status.velocity.x = m_lastCollision.direction.x * 0.6f;
					}
				}
			}
		}
		return;
	}

	// Attack
	AABB atk = (m_direction == CharacterDirection::Left ? m_attackRangeL : m_attackRangeR)[m_nAtk];
	atk.setSize(atk.size() * 1.25f);
	if (!m_status.attacking &&
		m_world->query<BodyType::Character>(atk + m_position).size()) {
		m_status.attacking = 1;
		m_timeToLastAtk = 0.0f;
	}
	else {
		m_timeToLastAtk -= dt;
	}
	if (m_timeToLastAtk >= 1000.0f) {
		m_nAtk = 0;
	}

	// Move Left
	if (target->getAABB().getPosition().x < m_aabb.getPosition().x /* &&
		m_world->query<BodyType::Static>(m_feetL + m_position).size() */) {
		m_status.moving = 1;
		m_direction = Left;

		if (m_status.onGround) {
			m_status.velocity.x = std::max(m_status.velocity.x - 0.01f, -0.5f);
		}
		else {
			m_status.velocity.x = std::max(m_status.velocity.x - 0.003f, -0.5f);
		}
	}
	// Move Right
	else if (target->getAABB().getPosition().x > m_aabb.getPosition().x /* &&
		m_world->query<BodyType::Static>(m_feetR + m_position).size()*/) {
		m_status.moving = 1;
		m_direction = Right;

		if (m_status.onGround) {
			m_status.velocity.x = std::min(m_status.velocity.x + 0.01f, 0.5f);
		}
		else {
			m_status.velocity.x = std::min(m_status.velocity.x + 0.003f, 0.5f);
		}
	}

	if (m_status.moving) {
		// Jump
		AABB jumpDetect = (m_direction == CharacterDirection::Left ? m_left : m_right);
		if (m_world->query<BodyType::Static>(jumpDetect + m_position).size() && m_status.onGround && !m_status.dead) {
			m_status.moving = 1;
			m_status.velocity.y = -1.5f;

			if (m_status.onWall) {
				m_status.velocity.x = 0;
				if (m_lastCollision.direction.x == 1 || m_lastCollision.direction.x == -1) {
					m_status.velocity.x = m_lastCollision.direction.x * 0.6f;
				}
			}
		}
	}
}

void BigMudGuard::myOnCollide(Body* other, const CollisionDetail& detail) {

}

AnimationType BigMudGuard::animationUpdate() {
	AnimationType ret = Idle;

	if (m_status.dead) {
		return None;
	}

	if (m_status.health <= 0) {
		m_status.dead = 1;
		return Death;
	}

	if (m_status.shocked) {
		return Hit;
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
			ret = Attack2;
			break;
		}
	}

	if (m_status.hit) {
		ret = Hit;
	}


	return ret;
}

void BigMudGuard::attack() {
	const AABB aabb = (m_direction == CharacterDirection::Right ? m_attackRangeR : m_attackRangeL)[m_nAtk] + m_position;

	if (!m_world) {
		return;
	}

	const std::vector<Body*> hit = m_world->query<BodyType::Character>(aabb);

	for (Body* body : hit) {
		if (m_hit[body]) {
			continue;
		}
		m_hit[body] = 1;
		body->getStatus().health -= m_config.attack;
		body->getStatus().hit = 1;
	}
}

void BigMudGuard::resetAttack() {
	m_hit.clear();
}
