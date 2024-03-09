#pragma once
#include <stack>
#include <vector>
#include <iostream>

#include "SFML/Graphics.hpp"

class GameState;

class StateManager {
private:
	enum OperationType {
		Push,
		Pop
	};
	struct Operation {
		OperationType type;
		GameState* state;
	};
public:
	StateManager();

	void pushState(GameState* state);
	void popState();
	GameState* topState();

	void update();
private:
	std::stack<GameState*> m_stateStack;
	std::stack<GameState*> m_toDelete;
};

class GameState {
public:
	GameState(StateManager& manager);
	virtual ~GameState() = default;

	virtual void onCreate();
	virtual void onDestroy();
	virtual void reset();
	virtual void handleEvent(sf::Event events);
	virtual void update(sf::Time dt);
	virtual void render(sf::RenderWindow& window);

	virtual bool shouldClose();

protected:
	StateManager& m_stateManager;
};