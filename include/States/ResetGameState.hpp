#pragma once

#include "State.hpp"

class ResetGameState : public GameState {
public:
	ResetGameState(StateManager& manager, sf::Texture frame);
	virtual void onCreate() override final;
	virtual void onDestroy() override final;
	virtual void reset() override final;
	virtual void update(sf::Time dt) override final;
	virtual void render(sf::RenderWindow& window) override final;

	virtual bool shouldClose() override final;
private:
	sf::Texture m_frame;

	sf::Shader m_shader;
};