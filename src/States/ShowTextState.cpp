#include "States/ShowTextState.hpp"

ShowTextState::ShowTextState(StateManager& manager, sf::Texture frame, const sf::Vector2f& textPosition, const std::string& text, float charDuration)
	: m_frame(frame)
	, m_progress(0.0f)
	, m_charDuration(charDuration)
	, m_position(textPosition)
	, m_string(text)
	, GameState(manager) {
	if (!m_font.loadFromFile("consola.ttf")) {
		std::cout << "Failed to load font" << "\n";
		return;
	}
	m_text.setFont(m_font);
	m_text.setString("");
	m_text.setCharacterSize(80);
	m_text.setStyle(sf::Text::Bold | sf::Text::Italic);
	m_text.setPosition(textPosition -
		sf::Vector2f{
			m_text.getGlobalBounds().width * 0.5f,
			m_text.getGlobalBounds().height * 0.5f
		}
	);
	m_text.setFillColor(sf::Color::Black);

	m_shader.loadFromFile("shaders/endScreen.frag", sf::Shader::Fragment);
}

void ShowTextState::onCreate() {}
void ShowTextState::onDestroy() {}
void ShowTextState::reset() {}
void ShowTextState::update(sf::Time dt) {
	m_progress += dt.asSeconds() * 1000.0f;

	int length = m_progress / m_charDuration + 1;
	if (length <= m_string.size()) {
		m_currentString = m_string.substr(0, length);
	}

	bool hasPressKey = 0;
	for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
		if (
			KeyState::getKey(static_cast<sf::Keyboard::Key>(i)).held &&
			!KeyState::getLastKey(static_cast<sf::Keyboard::Key>(i)).held ) {
			hasPressKey = 1;
			break;
		}
	}
	if (hasPressKey) {
		m_stateManager.popState();
	}
}

void ShowTextState::render(sf::RenderWindow& window) {
	sf::RectangleShape rect({
			static_cast<float>(window.getSize().x),
			static_cast<float>(window.getSize().y)
		});
	rect.setTexture(&m_frame);
	m_shader.setUniform("image", sf::Shader::CurrentTexture);
	m_shader.setUniform("transparency", 0.5f);
	window.draw(rect, &m_shader);

	m_text.setString(m_currentString);
	m_text.setCharacterSize(80);
	m_text.setStyle(sf::Text::Bold | sf::Text::Italic);
	m_text.setPosition(m_position -
		sf::Vector2f{
			m_text.getGlobalBounds().width * 0.5f,
			m_text.getGlobalBounds().height * 0.5f
		}
	);

	window.draw(m_text);
}

bool ShowTextState::shouldClose() {
	return 0;
}
