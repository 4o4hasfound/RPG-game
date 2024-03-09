#pragma once
#include "State.hpp"
#include "States/EndGameState.hpp"
#include "States/ShowTextState.hpp"

#include "KeyState.hpp"
#include "Animation.hpp"
#include "Character.hpp"
#include "TileMap.hpp"

#include "Physics/Body.hpp"
#include "Physics/Circle.hpp"
#include "Physics/PhysicsWorld.hpp"

#include "Characters/BACBot.hpp"
#include "Characters/ToasterBot.hpp"
#include "Characters/SpritBoxer.hpp"
#include "Enemies/DroidZapper.hpp"
#include "Enemies/StormHead.hpp"
#include "Enemies/MudGuard.hpp"
#include "Enemies/BigMudGuard.hpp"

#include "Objects/Thorn.hpp"
#include "Objects/Heal.hpp"
#include "Objects/TextPopup.hpp"
#include "Objects/WinArea.hpp"
#include "Objects/Chest.hpp"

class MainGameState : public GameState {
public:
	MainGameState(StateManager& manager);
	virtual void onCreate() override final;
	virtual void onDestroy() override final;
	virtual void reset() override final;
	virtual void update(sf::Time dt) override final;
	virtual void render(sf::RenderWindow& window) override final;

	virtual bool shouldClose() override final;

	const sf::RenderTexture& getFrame() const;
private:
	sf::RenderTexture m_frame;

	Character* m_character;
	Circle m_updateRange;
	TileMap m_map;
	PhysicsWorld m_world;
	std::vector<Enemy*> m_enemies;
	std::vector<Body*> m_obstacles;
	std::vector<Body*> m_thorns;
	std::vector<Body*> m_heal;
	std::vector<Body*> m_texts;
	std::vector<Body*> m_winAreas;
	std::vector<Chest*> m_chests;

	sf::Font m_font;
	sf::Text m_score;

	void loadFont();
	void loadEnemies();
	void loadTextPopups();
	void loadHeal();
	void loadObjects();
	void loadObstacles();
	void loadWinArea();
	void loadChests();

	void initCharacter();
	void renderScore(sf::RenderTarget& window);

	void deleteObjects();
};