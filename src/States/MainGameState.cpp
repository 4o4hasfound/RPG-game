#include "States/MainGameState.hpp"

MainGameState::MainGameState(StateManager& manager) : GameState(manager), m_map("Maps/MagicLand.tmx"), m_updateRange(0.0f, 0.0f, 1000.0f) {
	srand(time(NULL));
	m_frame.create(1, 1);

	loadFont();
	initCharacter();
	loadObstacles();
	loadEnemies();
	loadObjects();
	loadTextPopups();
	loadHeal();
	loadWinArea();
	loadChests();
}
void MainGameState::onCreate() {}
void MainGameState::onDestroy() {}
void MainGameState::reset() {
	deleteObjects();

	initCharacter();
	loadObstacles();
	loadEnemies();
	loadObjects();
	loadTextPopups();
	loadHeal();
	loadWinArea();
	loadChests();
}
void MainGameState::update(sf::Time dt) {
	m_map.update(dt);
	m_character->update(dt.asSeconds() * 1000.0f);
	for (Chest* chest : m_chests) {
		chest->update(dt.asSeconds() * 1000.0f);
	}
	const auto enemies = m_world.query<BodyType::Enemy>(m_updateRange + m_character->getPosition());
	for (auto enemy : m_enemies) {
		enemy->setFreezed(1);
	}
	for (auto enemy : enemies) {
		enemy->setFreezed(0);
		dynamic_cast<Enemy*>(enemy)->update(dt.asSeconds() * 1000.0f);
	}
	m_world.applyGravity(dt.asSeconds() * 1000.0f);
	m_world.update();

	if (m_character->getStatus().dead && !m_character->deadAnimationPlaying) {
		m_frame.display();
		m_stateManager.pushState(new EndGameState(
			m_stateManager,
			m_frame.getTexture(),
			sf::Vector2f{
				static_cast<float>(m_frame.getSize().x) * 0.5f,
				static_cast<float>(m_frame.getSize().y) * 0.5f,
			},
			"You died QAQ\nPress any key to respawn",
			50.0f
		));
	}
}

void MainGameState::render(sf::RenderWindow& window) {
	if (window.getSize() != m_frame.getSize()) {
		m_frame.clear();
		m_frame.create(window.getSize().x, window.getSize().y);
	}
	m_frame.clear();
	sf::View view = m_frame.getView();
	sf::Vector2f position = m_character->getPosition() + m_character->getConfig().symmetryAxis * m_character->getConfig().scale;
	position.x = std::floor(position.x);
	position.y = std::floor(position.y);
	position.x += 0.375f;
	position.y += 0.375f;

	position.x += ((rand() % 10) * 0.1 - 0.5) * 5.0f * m_character->windowTremble;
	position.y += ((rand() % 10) * 0.1 - 0.5) * 5.0f * m_character->windowTremble;

	view.setCenter(position);
	view.setSize(
		sf::Vector2f{ 
			static_cast<float>(m_frame.getSize().x),
			static_cast<float>(m_frame.getSize().y),
		} * 0.75f);
	m_frame.setView(view);
	m_map.draw(m_frame);
	for (auto enemy : m_enemies) {
		enemy->draw(m_frame);
	}
	for (Chest* chest : m_chests) {
		chest->draw(m_frame);
	}
	m_character->draw(m_frame);
	renderScore(m_frame);
	m_frame.display();

	sf::Sprite screen(m_frame.getTexture());

	window.draw(screen);
	//m_screenOverlayTexture.draw(rect, &m_fragment);
	//m_screenOverlay.setTexture(m_screenOverlayTexture.getTexture());
	//window.draw(rect, &m_fragment);

}

bool MainGameState::shouldClose() {
	return 0;
}

const sf::RenderTexture& MainGameState::getFrame() const {
	return m_frame;
}

void MainGameState::loadFont() {
	if (!m_font.loadFromFile("consola.ttf")) {
		std::cout << "Error loading consola.ttf" << "\n";
		return;
	}
	m_score.setFont(m_font);
	m_score.setPosition(50, 50);
	m_score.setStyle(sf::Text::Bold | sf::Text::Italic);
}

void MainGameState::loadEnemies() {
	const auto& stormheads = m_map.getLayerCoords("StormHead");
	for (const sf::Vector2f& coord : stormheads) {
		m_enemies.emplace_back(new StormHead(&m_world));
		m_enemies.back()->setPosition(coord);
		if (rand() % 2) {
			m_enemies.back()->setDirection(CharacterDirection::Left);
		}
	}
	const auto& mudguards = m_map.getLayerCoords("MudGuard");
	for (const sf::Vector2f& coord : mudguards) {
		m_enemies.emplace_back(new MudGuard(&m_world));
		m_enemies.back()->setPosition(coord);
		if (rand() % 2) {
			m_enemies.back()->setDirection(CharacterDirection::Left);
		}
	}
	const auto& bigmudguards = m_map.getLayerCoords("BigMudGuard");
	for (const sf::Vector2f& coord : bigmudguards) {
		m_enemies.emplace_back(new BigMudGuard(&m_world));
		m_enemies.back()->setPosition(coord);
		if (rand() % 2) {
			m_enemies.back()->setDirection(CharacterDirection::Left);
		}
	}
	const auto& droids = m_map.getLayerCoords("DroidZapper");
	for (const sf::Vector2f& coord : droids) {
		m_enemies.emplace_back(new DroidZapper(&m_world));
		m_enemies.back()->setPosition(coord);
		if (rand() % 2) {
			m_enemies.back()->setDirection(CharacterDirection::Left);
		}
	}

	for (auto enemy : m_enemies) {
		m_world.addBody(enemy);
	}
}

void MainGameState::loadTextPopups() {
	const auto& text0 = m_map.getLayerObjects("Text0");
	m_texts.emplace_back(new TextPopup(
		text0[0], 
		m_stateManager, 
		std::vector<std::string>{
			"Unknown voice:\nHello!\nI am your anonymous guide\nand I will teach you\nhow to play this game\n(Press any key to continue)", 
			"Press 'a' and 'd'\nto move left and right\nand 'w' to jump", 
			"You can jump off a wall\nby jumping on a wall", 
			"To attack\nyou will need to left click\nor hold left click", 
			"And to dash\nyou can right click", 
			"There are multiple treasures\nin the map",
			"Try to\nexplore the map\nand find them all!",
			"I'll give you hint\nin some places", 
			"And good luck\nbeating the game!"
		}
	));
	m_world.addBody(m_texts.back());

	const auto& text1 = m_map.getLayerObjects("Text1");
	m_texts.emplace_back(new TextPopup(
		text1[0], 
		m_stateManager, 
		std::vector<std::string>{
			"Unknown voice:\nMaybe you should go up\nin the trees :)", 
			"You can try to\ndash from the high point"
		}
	));
	m_world.addBody(m_texts.back());

	const auto& text2 = m_map.getLayerObjects("Text2");
	m_texts.emplace_back(new TextPopup(
		text2[0], 
		m_stateManager, 
		std::vector<std::string>{
			"Unknown voice:\nCongratulations\nfor getting here!", 
			"You can rest here\nand your health\nwill regenerate", 
			"Take good use of this place\nBecause the enemies ahead\nare very strong"
		}
	));
	m_world.addBody(m_texts.back());

	const auto& text3 = m_map.getLayerObjects("Text3");
	m_texts.emplace_back(new TextPopup(
		text3[0], 
		m_stateManager, 
		std::vector<std::string>{
			"Unknown voice:\nThe only way\nof entering the castle", 
			"is to go to\nthe right most branch\ngood luck"
		}
	));
	m_world.addBody(m_texts.back());

	const auto& text4 = m_map.getLayerObjects("Text4");
	m_texts.emplace_back(new TextPopup(
		text4[0],
		m_stateManager,
		std::vector<std::string>{
		"Unknown voice:\nGet up the tower\nto finish the game!",
			"Quick!"
	}
	));
	m_world.addBody(m_texts.back());

	const auto& text5 = m_map.getLayerObjects("Text5");
	m_texts.emplace_back(new TextPopup(
		text5[0],
		m_stateManager,
		std::vector<std::string>{
		"Unknown voice:\nMaybe there are\nsomething up there",
		"Just saying"
	}
	));
	m_world.addBody(m_texts.back());
}

void MainGameState::loadHeal() {
	const auto& heals = m_map.getLayerObjects("Heal");
	for (const auto& aabb : heals) {
		m_heal.emplace_back(new Heal(aabb));
		m_world.addBody(m_heal.back());
	}
}

void MainGameState::loadObjects() {
	const auto& thorns = m_map.getLayerObjects("damage");
	for (const auto& aabb : thorns) {
		m_thorns.emplace_back(new Thorn(aabb));
		m_world.addBody(m_thorns.back());
	}
}

void MainGameState::loadObstacles() {
	const auto& objects = m_map.getLayerObjects("ground");
	for (const AABB& obstacle : objects) {
		m_obstacles.push_back(new Body(BodyType::Static, obstacle));
		m_world.addBody(m_obstacles.back());
	}
}

void MainGameState::loadWinArea() {
	const auto& winArea = m_map.getLayerObjects("Win");
	for (const AABB& area : winArea) {
		m_winAreas.push_back(new WinArea(
			area,
			m_stateManager, 
			std::vector<std::string>{
				"Congratulations!\nYou have reach the end\nof this game!",
				"There are some\n'not so secret' secret path\nin the map",
				"And of course\nsome treasure chests",
				"Did you find them all?",
				"If not\nyou can replay\nby pressing any key"
			}
		));
		m_world.addBody(m_winAreas.back());
	}
}

void MainGameState::loadChests() {
	const auto& chests = m_map.getLayerCoords("Chests");
	for (const auto& chest : chests) {
		m_chests.emplace_back(new Chest());
		m_chests.back()->setPosition(chest);
		m_world.addBody(m_chests.back());
	}
}

void MainGameState::initCharacter() {
	m_character = new SpritBoxer(&m_world);
	m_character->setPosition(m_map.getLayerCoords("Player")[0]);
	m_world.addBody(m_character);
}

void MainGameState::renderScore(sf::RenderTarget& window) {
	int opened = 0;
	for (Chest* chest : m_chests) {
		if (chest->getOpened()) {
			++opened;
		}
	}
	m_score.setString("Chest collected: " + std::to_string(opened) + " / " + std::to_string(m_chests.size()));
	m_score.setCharacterSize(40);
	m_score.setPosition(window.getView().getCenter() - window.getView().getSize() * 0.45f);
	m_score.setStyle(sf::Text::Bold | sf::Text::Italic);
	window.draw(m_score);
}

void MainGameState::deleteObjects() {
	for (auto enemy : m_enemies) {
		delete enemy;
	}
	std::vector<Enemy*>().swap(m_enemies);

	for (auto obstacle : m_obstacles) {
		delete obstacle;
	}
	std::vector<Body*>().swap(m_obstacles);

	for (auto thorn : m_thorns) {
		delete thorn;
	}
	std::vector<Body*>().swap(m_thorns);

	for (auto heal : m_heal) {
		delete heal;
	}
	std::vector<Body*>().swap(m_heal);

	for (auto text : m_texts) {
		delete text;
	}
	std::vector<Body*>().swap(m_texts);

	for (auto area : m_winAreas) {
		delete area;
	}
	std::vector<Body*>().swap(m_winAreas);

	for (auto chest : m_chests) {
		delete chest;
	}
	std::vector<Chest*>().swap(m_chests);

	delete m_character;
	m_world.clear();
}
