#pragma once

#include <random>
#include <time.h>
#include <SFML/Graphics.hpp>
#include "MapTileLayer.hpp"

#include "State.hpp"
#include "KeyState.hpp"
#include "Animation.hpp"
#include "Character.hpp"
#include "TileMap.hpp"
#include "Physics/Body.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "States/MainGameState.hpp"

class Game {
public:
	Game();
	void run();
private:
	sf::RenderWindow m_window;
	StateManager m_state;

	void handleEvents();
};