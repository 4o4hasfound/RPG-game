#include "Game.hpp"

Game::Game()
	: m_window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "SFML window") {
	sf::Texture texture;
	m_state.pushState(new MainGameState(m_state));

}

void Game::run() {
	sf::Clock dtClock;
	while (m_window.isOpen()) {
		handleEvents();
		m_window.clear(sf::Color::White);
		m_state.update();
		GameState* state = m_state.topState();

		const sf::Time dt = dtClock.restart();
		state->update(dt);
		
		state->render(m_window);

		m_window.display();

		if (state->shouldClose()) {
			m_window.close();
		}
	}
}

void Game::handleEvents() {
	KeyState::update(m_window);
	
	sf::Event events;
	while (m_window.pollEvent(events)) {
		m_state.topState()->handleEvent(events);

		if (events.type == sf::Event::Closed) {
			m_window.close();
		}
	}
}
