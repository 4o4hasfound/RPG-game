#include "Enemies/StormHead.hpp"

const EnemyConfig StormHead::s_config{
	100.0f,		// health
	50.0f,		// attack
	0.2f,		// speed

	3.0f,		// scale
	sf::Vector2f{53.0f, 105.0f},
	sf::Vector2f{19.0f, 36.0f}		// size
};

StormHead::StormHead(PhysicsWorld* world)
	: Enemy(s_config, world), m_nAtk(0) {
	initAnimation();
}

void StormHead::initAnimation() {
	m_animation.addAnimation(
		Idle,
		Animation("Characters/stormhead/idle.png", 124, 160, TopDown)
	);
	m_animation.addAnimation(
		Walk,
		Animation("Characters/stormhead/run.png", 124, 120, TopDown)
	);
	m_animation.addAnimation(
		Attack,
		Animation("Characters/stormhead/attack.png", 124, 80, TopDown),
		[this]() { m_status.attacking = 0; }
	);
	m_animation.addAnimation(
		Hit,
		Animation("Characters/stormhead/damaged.png", 124, 120, TopDown),
		[this]() { m_status.hit = 0; }
	);
	m_animation.addAnimation(
		Death,
		Animation("Characters/stormhead/death.png", 124, 120, TopDown),
		[this]() { m_status.attacking = 0; }
	);

	m_animation.setCurrentAnimation(Idle);
	m_animation.setUpdateFunction(std::bind(&StormHead::animationUpdate, this));
	m_animation.addKeyFrameCallback(Attack, 8, [this]() { m_nAtk = 0; attack(); });
	m_animation.addKeyFrameCallback(Attack, 9, [this]() {m_nAtk = 1; resetAttack(); });

	m_animation.addKeyFrameCallback(Attack, 12, [this]() { m_nAtk = 1; attack(); });
	m_animation.addKeyFrameCallback(Attack, 13, [this]() {m_nAtk = 0; resetAttack(); });
}

void StormHead::myUpdate(float dt) {
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

	// Move away from player
	if (std::abs(m_line.getStart().x - m_line.getEnd().x) < 55.0f * s_config.scale * 0.75f) {
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
	std::vector<AABB> atk = (m_direction == CharacterDirection::Left ? m_attackRangeL : m_attackRangeR)[m_nAtk];
	bool hit = 0;
	for (const AABB& aabb : atk) {
		AABB range = aabb + m_position;
		range.setSize(range.size() * 1.25f);
		if (!m_status.attacking &&
			m_world->query<BodyType::Character>(range).size()) {
			hit = 1;
			break;
		}
	}
	if (hit) {
		m_status.attacking = 1;
	}
	// Move Left
	if (target->getAABB().getPosition().x < m_aabb.getPosition().x /* &&
		!m_status.attacking &&
		m_world->query<BodyType::Static>(m_feetL + m_position).size()*/) {
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
		!m_status.attacking &&
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

void StormHead::myOnCollide(Body* other, const CollisionDetail& detail) {

}

AnimationType StormHead::animationUpdate() {
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
		ret = Attack;
	}

	if (m_status.hit) {
		ret = Hit;
	}


	return ret;
}

void StormHead::attack() {
	if (!m_world) {
		return;
	}
	for (const AABB& atkRange: (m_direction == CharacterDirection::Right ? m_attackRangeR : m_attackRangeL)[m_nAtk]) {
		const AABB aabb = atkRange + m_position;

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
}

void StormHead::resetAttack() {
	m_hit.clear();
}