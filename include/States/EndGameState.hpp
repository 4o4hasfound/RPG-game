#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#include "State.hpp"
#include "KeyState.hpp"

class EndGameState : public GameState {
public:
	EndGameState(StateManager& manager, sf::Texture frame, const sf::Vector2f& textPosition, const std::string& text, float charDuration);
	virtual void onCreate() override final;
	virtual void onDestroy() override final;
	virtual void reset() override final;
	virtual void update(sf::Time dt) override final;
	virtual void render(sf::RenderWindow& window) override final;

	virtual bool shouldClose() override final;
private:
	sf::Texture m_frame;
	sf::Font m_font;
	sf::Text m_text;

	sf::Shader m_shader;

	float m_progress;
	float m_charDuration;
	sf::Vector2f m_position;
	std::string m_string;
	std::string m_currentString;
};