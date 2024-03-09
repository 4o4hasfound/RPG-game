#include "States/ResetGameState.hpp"

ResetGameState::ResetGameState(StateManager& manager, sf::Texture frame)
	: m_frame(frame)
	, GameState(manager) {
	m_shader.loadFromFile("shaders/endScreen.frag", sf::Shader::Fragment);
}

void ResetGameState::onCreate() {}
void ResetGameState::onDestroy() {}
void ResetGameState::reset() {}
void ResetGameState::update(sf::Time dt) {
	m_stateManager.popState();
	m_stateManager.topState()->reset();
}

void ResetGameState::render(sf::RenderWindow& window) {
	sf::RectangleShape rect({
			static_cast<float>(window.getSize().x),
			static_cast<float>(window.getSize().y)
		});
	rect.setTexture(&m_frame);
	m_shader.setUniform("image", sf::Shader::CurrentTexture);
	m_shader.setUniform("transparency", 0.5f);
	window.draw(rect, &m_shader);
}

bool ResetGameState::shouldClose() {
	return 0;
}
