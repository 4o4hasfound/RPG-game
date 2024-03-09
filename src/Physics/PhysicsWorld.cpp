#include "Physics/PhysicsWorld.hpp"

PhysicsWorld::PhysicsWorld() {
	m_grid.resize(2);
	for (int i = 0; i < 2; ++i) {
		m_grid[i].resize(2);
	}
}
PhysicsWorld::~PhysicsWorld() {

}

void PhysicsWorld::addBody(Body* body) {
	addObjectInGrid(m_bodies.size(), body->getAABB());
	m_bodies.push_back(body);
	m_aabbs.push_back(body->getAABB());
}

void PhysicsWorld::update() {
	updateGrid();
	globalCollisionDetection();
}

void PhysicsWorld::renderAABB(sf::RenderTarget& window, const AABB& aabb) {
	const int x1 = (aabb.left() - m_globalBound.left()) / m_chunkSize.x;
	const int x2 = (aabb.right() - m_globalBound.left()) / m_chunkSize.x;
	const int y1 = (aabb.top() - m_globalBound.top()) / m_chunkSize.y;
	const int y2 = (aabb.bottom() - m_globalBound.top()) / m_chunkSize.y;
	sf::RectangleShape rect(m_chunkSize);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(5);
	rect.setOutlineColor(sf::Color::Red);
	for (int y = y1; y <= y2; ++y) {
		for (int x = x1; x <= x2; ++x) {
			rect.setPosition(sf::Vector2f(m_globalBound.left() + x * m_chunkSize.x, m_globalBound.top() + y * m_chunkSize.y));
			for (const int ind : m_grid[y][x]) {
				m_bodies[ind]->getAABB().draw(window);
			}
			window.draw(rect);
		}
	}
}

void PhysicsWorld::clear() {
	for (auto& row : m_grid) {
		for (auto& list : row) {
			Chunk().swap(list);
		}
		std::vector<Chunk>().swap(row);
	}
	std::vector<std::vector<Chunk>>().swap(m_grid);

	std::vector<Body*>().swap(m_bodies);
	std::vector<AABB>().swap(m_aabbs);
	m_globalBound = { {-m_chunkSize.x, -m_chunkSize.y}, m_chunkSize * 2.0f };
}

void PhysicsWorld::addObjectInGrid(int index, const AABB& aabb) {
	if (!inBound(aabb)) {
		expandGrid(aabb);
	}
	const int x1 = (aabb.left() - m_globalBound.left()) / m_chunkSize.x;
	const int x2 = (aabb.right() - m_globalBound.left()) / m_chunkSize.x;
	const int y1 = (aabb.top() - m_globalBound.top()) / m_chunkSize.y;
	const int y2 = (aabb.bottom() - m_globalBound.top()) / m_chunkSize.y;
	for (int y = y1; y <= y2; ++y) {
		for (int x = x1; x <= x2; ++x) {
			m_grid[y][x].push(index);
		}
	}
}

void PhysicsWorld::removeObjectInGrid(int index, const AABB& aabb) {
	const int x1 = (aabb.left() - m_globalBound.left()) / m_chunkSize.x;
	const int x2 = (aabb.right() - m_globalBound.left()) / m_chunkSize.x;
	const int y1 = (aabb.top() - m_globalBound.top()) / m_chunkSize.y;
	const int y2 = (aabb.bottom() - m_globalBound.top()) / m_chunkSize.y;

	for (int y = y1; y <= y2; ++y) {
		for (int x = x1; x <= x2; ++x) {
			auto itr = m_grid[y][x].find(index);
			m_grid[y][x].remove(itr);
		}
	}
}

bool PhysicsWorld::inBound(const AABB& aabb) const {
	return m_globalBound.contain(aabb);
}

void PhysicsWorld::updateGrid() {
	for (int i = 0; i < m_bodies.size(); ++i) {
		Body* body = m_bodies[i];
		if (body->getType() == BodyType::Static || body->getType() == BodyType::Static_Uncollidable || body->getAABB() == m_aabbs[i]) {
			continue;
		}
		removeObjectInGrid(i, m_aabbs[i]);
		addObjectInGrid(i, body->getAABB());
		m_aabbs[i] = body->getAABB();
	}
}

void PhysicsWorld::applyGravity(float dt) {
	for (Body* body : m_bodies) {
		if (body->getFreezed() || body->getType() == BodyType::Static || body->getType() == BodyType::Static_Uncollidable) {
			continue;
		}
		body->m_status.velocity.y += m_gravity * dt;
	}
}

template<BodyType ToQuery>
std::vector<Body*> PhysicsWorld::query(const AABB& aabb) {
	std::unordered_map<Body*, bool> inVector;
	std::vector<Body*> ret;
	const int x1 = std::max(0.0f, (aabb.left() - m_globalBound.left()) / m_chunkSize.x);
	const int x2 = std::min(static_cast<float>(m_grid[0].size() - 1), (aabb.right() - m_globalBound.left()) / m_chunkSize.x);
	const int y1 = std::max(0.0f, (aabb.top() - m_globalBound.top()) / m_chunkSize.y);
	const int y2 = std::min(static_cast<float>(m_grid.size() - 1), (aabb.bottom() - m_globalBound.top()) / m_chunkSize.y);

	for (int y = y1; y <= y2; ++y) {
		for (int x = x1; x <= x2; ++x) {
			for (int index : m_grid[y][x]) {
				if (!aabb.intersect(m_bodies[index]->getAABB())) {
					continue;
				}
				if ((ToQuery != BodyType::None && m_bodies[index]->getType() != ToQuery) || m_bodies[index]->getType() == BodyType::Static_Uncollidable) {
					continue;
				}
				if (inVector[m_bodies[index]]) {
					continue;
				}
				inVector[m_bodies[index]] = 1;
				ret.push_back(m_bodies[index]);
			}
		}
	}
	return ret;
}
template std::vector<Body*> PhysicsWorld::query<BodyType::None>(const AABB& aabb);
template std::vector<Body*> PhysicsWorld::query<BodyType::Static>(const AABB& aabb);
template std::vector<Body*> PhysicsWorld::query<BodyType::Character>(const AABB& aabb);
template std::vector<Body*> PhysicsWorld::query<BodyType::Enemy>(const AABB& aabb);

template<BodyType ToQuery>
std::vector<Body*> PhysicsWorld::query(const Circle& circle) {
	std::unordered_map<Body*, bool> inVector;
	std::vector<Body*> ret;
	const int x1 = std::max(0.0f, (circle.getPosition().x - circle.getRadius() - m_globalBound.left()) / m_chunkSize.x);
	const int x2 = std::min(m_grid[0].size()-1.0f, (circle.getPosition().x + circle.getRadius() - m_globalBound.left()) / m_chunkSize.x);
	const int y1 = std::max(0.0f, (circle.getPosition().y - circle.getRadius() - m_globalBound.top()) / m_chunkSize.y);
	const int y2 = std::min(m_grid.size() - 1.0f, (circle.getPosition().y + circle.getRadius() - m_globalBound.top()) / m_chunkSize.y);

	for (int y = y1; y <= y2; ++y) {
		for (int x = x1; x <= x2; ++x) {
			for (int index : m_grid[y][x]) {
				if (!circle.intersect(m_bodies[index]->getAABB())) {
					continue;
				}
				if ((ToQuery != BodyType::None && m_bodies[index]->getType() != ToQuery) || m_bodies[index]->getType() == BodyType::Static_Uncollidable) {
					continue;
				}
				if (inVector[m_bodies[index]]) {
					continue;
				}
				inVector[m_bodies[index]] = 1;
				ret.push_back(m_bodies[index]);
			}
		}
	}
	return ret;
}
template std::vector<Body*> PhysicsWorld::query<BodyType::None>(const Circle& circle);
template std::vector<Body*> PhysicsWorld::query<BodyType::Static>(const Circle& circle);
template std::vector<Body*> PhysicsWorld::query<BodyType::Character>(const Circle& circle);
template std::vector<Body*> PhysicsWorld::query<BodyType::Enemy>(const Circle& circle);

template<BodyType ToQuery>
std::vector<Body*> PhysicsWorld::query(const Line& line) {
	std::unordered_map<Body*, bool> inVector;
	std::vector<Body*> ret;
	int x1 = (line.getStart().x - m_globalBound.left()) / m_chunkSize.x;
	int x2 = (line.getEnd().x - m_globalBound.left()) / m_chunkSize.x;
	if (x2 < x1) std::swap(x1, x2);
	x1 = std::max(0, x1);
	x2 = std::min(static_cast<int>(m_grid[0].size() - 1), x2);
	int y1 = (line.getStart().y - m_globalBound.top()) / m_chunkSize.y;
	int y2 = (line.getEnd().y - m_globalBound.top()) / m_chunkSize.y;
	if (y2 < y1) std::swap(y1, y2);
	y1 = std::max(0, y1);
	y2 = std::min(static_cast<int>(m_grid.size() - 1), y2);

	for (int y = y1; y <= y2; ++y) {
		for (int x = x1; x <= x2; ++x) {
			for (int index : m_grid[y][x]) {
				if (!line.intersect(m_bodies[index]->getAABB())) {
					continue;
				}
				if ((ToQuery != BodyType::None && m_bodies[index]->getType() != ToQuery) || m_bodies[index]->getType() == BodyType::Static_Uncollidable) {
					continue;
				}
				if (inVector[m_bodies[index]]) {
					continue;
				}
				inVector[m_bodies[index]] = 1;
				ret.push_back(m_bodies[index]);
			}
		}
	}
	return ret;
}
template std::vector<Body*> PhysicsWorld::query<BodyType::None>(const Line& line);
template std::vector<Body*> PhysicsWorld::query<BodyType::Static>(const Line& line);
template std::vector<Body*> PhysicsWorld::query<BodyType::Character>(const Line& line);
template std::vector<Body*> PhysicsWorld::query<BodyType::Enemy>(const Line& line);

void PhysicsWorld::globalCollisionDetection() {
	std::unordered_map<std::pair<int, int>, bool, PairHash<int, int>> solved;

	solved.reserve(m_bodies.size() * m_bodies.size());
	for (int i = 0; i < m_bodies.size(); ++i) {
		Body* body = m_bodies[i];
		if (body->getType() == BodyType::Static || body->getType() == BodyType::Static_Uncollidable || body->getFreezed()) {
			continue;
		}

		const int x1 = (body->getAABB().left() - m_globalBound.left()) / m_chunkSize.x;
		const int x2 = (body->getAABB().right() - m_globalBound.left()) / m_chunkSize.x;
		const int y1 = (body->getAABB().top() - m_globalBound.top()) / m_chunkSize.y;
		const int y2 = (body->getAABB().bottom() - m_globalBound.top()) / m_chunkSize.y;
		//Logger::Log<Debug>(x1, x2, y1, y2);
		for (int y = y1; y <= y2; ++y) {
			for (int x = x1; x <= x2; ++x) {
				localCollisionDetection(i, m_grid[y][x], solved);
			}
		}
	}
}

void PhysicsWorld::localCollisionDetection(int bodyIndex, const Chunk& chunk, std::unordered_map<std::pair<int, int>, bool, PairHash<int, int> >& solved) {
	Body* body = m_bodies[bodyIndex];
	for (int i : chunk) {
		//Logger::Log<Debug>(i);
		Body* other = m_bodies[i];

		if (bodyIndex == i || solved[std::make_pair(bodyIndex, i)] || solved[std::make_pair(i, bodyIndex)]) {
			continue;
		}
		solved[std::make_pair(bodyIndex, i)] = solved[std::make_pair(i, bodyIndex)] = 1;

		if (!body->getAABB().intersect(other->getAABB())) {
			continue;
		}
		if (other->getType() == BodyType::Static_Uncollidable) {
			body->onCollide(other, CollisionDetail{});
			other->onCollide(body, CollisionDetail{});
			continue;
		}

		const float deltax1 = other->getAABB().right() - body->getAABB().left();
		const float deltax2 = body->getAABB().right() - other->getAABB().left();

		const float deltay1 = other->getAABB().bottom() - body->getAABB().top();
		const float deltay2 = body->getAABB().bottom() - other->getAABB().top();

		// auto jump
		if (deltay2 < 35.0f && std::abs(deltay2) < std::abs(deltay1)) {
			const sf::Vector2f direction{ 0.0, -1.0 };
			if (other->getType() == BodyType::Static) {
				body->move(0, -deltay2);
				body->m_status.velocity.y = 0;
				body->m_status.onGround = 1;
				body->onCollide(other, CollisionDetail{ direction, deltay2 });
				other->onCollide(body, CollisionDetail{ {}, deltay2 });
				continue;
			}
			body->onCollide(other, CollisionDetail{ direction, deltay2 });
			other->onCollide(body, CollisionDetail{ -direction, deltay2 });
			continue;
		}

		// Move the bodies along the x axis
		if (std::min(deltax1, deltax2) < std::min(deltay1, deltay2)) {

			// 'body' is at the right of 'other'
			if (std::abs(deltax1) < std::abs(deltax2)) {
				const sf::Vector2f direction{ 1.0, 0.0 };
				if (other->getType() == BodyType::Static) {
					body->move(deltax1, 0);
					body->onCollide(other, CollisionDetail{ direction, deltax1 });
					other->onCollide(body, CollisionDetail{ {}, deltax1 });
					continue;
				}
				body->onCollide(other, CollisionDetail{ direction, deltax1 });
				other->onCollide(body, CollisionDetail{ -direction, deltax1 });
				continue;
			}
			// 'body' is at the left of 'other'
			else {
				const sf::Vector2f direction{ -1.0, 0.0 };
				if (other->getType() == BodyType::Static) {
					body->move(-deltax2, 0);
					body->onCollide(other, CollisionDetail{ direction, deltax2 });
					other->onCollide(body, CollisionDetail{ {}, deltax2 });
					continue;
				}
				body->onCollide(other, CollisionDetail{ direction, deltax2 });
				other->onCollide(body, CollisionDetail{ -direction, deltax2 });
				continue;
			}
		}
		// Move the bodies along the y axis
		else {

			// 'body' is at the bottom of 'other'
			if (std::abs(deltay1) < std::abs(deltay2)) {
				const sf::Vector2f direction{ 0.0, 1.0 };
				if (other->getType() == BodyType::Static) {
					body->move(0, deltay1);
					body->m_status.velocity.y = 0;
					body->onCollide(other, CollisionDetail{ direction, deltay1 });
					other->onCollide(body, CollisionDetail{ {}, deltay1 });
					continue;
				}
				body->onCollide(other, CollisionDetail{ direction, deltay1 });
				other->onCollide(body, CollisionDetail{ -direction, deltay1 });
				continue;
			}
			// 'body' is at the top of 'other'
			else {
				const sf::Vector2f direction{ 0.0, -1.0 };
				if (other->getType() == BodyType::Static) {
					body->move(0, -deltay2);
					body->m_status.velocity.y = 0;
					body->m_status.onGround = 1;
					body->onCollide(other, CollisionDetail{ direction, deltay2 });
					other->onCollide(body, CollisionDetail{ {}, deltay2 });
					continue;
				}
				body->onCollide(other, CollisionDetail{ direction, deltay2 });
				other->onCollide(body, CollisionDetail{ -direction, deltay2 });
				continue;
			}
		}
	}
}


void PhysicsWorld::expandGrid(const AABB& aabb) {
	expandGridVertical(aabb);
	expandGridHorizontal(aabb);
}

void PhysicsWorld::expandGridHorizontal(const AABB& aabb) {
	enum ExpandDirection { Up, Down, Left, Right, UpDown, LeftRight, None };

	const AABB oldBound = m_globalBound;

	ExpandDirection horizontal = None;
	if (aabb.left() <= m_globalBound.left()) {
		horizontal = Left;
	}
	if (aabb.right() >= m_globalBound.right()) {
		horizontal = horizontal == Left ? LeftRight : Right;
	}

	if (horizontal == Left || horizontal == LeftRight) {
		const int factor = std::floor(-(aabb.left() - m_globalBound.left()) / m_chunkSize.x) + 1;
		m_globalBound.setLeft(m_globalBound.left() - m_chunkSize.x * factor);
	}
	if (horizontal == Right || horizontal == LeftRight) {
		const int factor = std::floor((aabb.right() - m_globalBound.right()) / m_chunkSize.x) + 1;
		m_globalBound.setRight(m_globalBound.right() + m_chunkSize.x * factor);
	}

	if (m_globalBound != oldBound) {
		for (auto& vec : m_grid) {
			vec.resize(m_globalBound.width() / m_chunkSize.x);
		}
		const int delta = (oldBound.left() - m_globalBound.left()) / m_chunkSize.x;
		for (int y = 0; y < m_grid.size(); ++y) {
			for (int x = oldBound.width() / m_chunkSize.x - 1; x >= 0; --x) {
				m_grid[y][x + delta].swap(m_grid[y][x]);
			}
		}
	}
}

void PhysicsWorld::expandGridVertical(const AABB& aabb) {
	enum ExpandDirection { Up, Down, Left, Right, UpDown, LeftRight, None };

	const AABB oldBound = m_globalBound;

	ExpandDirection vertical = None;
	if (aabb.top() <= m_globalBound.top()) {
		vertical = Up;
	}
	if (aabb.bottom() >= m_globalBound.bottom()) {
		vertical = vertical == Up ? UpDown : Down;
	}

	if (vertical == Up || vertical == UpDown) {
		const int factor = std::floor(-(aabb.top() - m_globalBound.top()) / m_chunkSize.y) + 1;
		m_globalBound.setTop(m_globalBound.top() - m_chunkSize.y * factor);
	}
	if (vertical == Down || vertical == UpDown) {
		const int factor = std::floor((aabb.bottom() - m_globalBound.bottom()) / m_chunkSize.y) + 1;
		m_globalBound.setBottom(m_globalBound.bottom() + m_chunkSize.y * factor);
	}

	if (m_globalBound != oldBound) {
		m_grid.resize(m_globalBound.height() / m_chunkSize.y);
		for (auto& vec : m_grid) vec.resize(m_globalBound.width() / m_chunkSize.x);
		const int delta = (oldBound.top() - m_globalBound.top()) / m_chunkSize.y;
		for (int y = oldBound.height() / m_chunkSize.y - 1; y >= 0; --y) {
			m_grid[y + delta].swap(m_grid[y]);
		}
	}
}