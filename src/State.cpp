#include "State.hpp"

StateManager::StateManager() {

}

void StateManager::pushState(GameState* state) {
	state->onCreate();
	m_stateStack.push(state);
}

void StateManager::popState() {
	m_stateStack.top()->onDestroy();
	m_toDelete.push(m_stateStack.top());
	m_stateStack.pop();
}

GameState* StateManager::topState() {
	return m_stateStack.top();
}

void StateManager::update() {
	while (m_toDelete.size()) {
		delete m_toDelete.top();
		m_toDelete.pop();
	}
}

GameState::GameState(StateManager& manager) 
	: m_stateManager(manager) {

}

void GameState::onCreate(){
}

void GameState::onDestroy(){
}

void GameState::reset(){
}

void GameState::handleEvent(sf::Event events){
}

void GameState::update(sf::Time dt){
}

void GameState::render(sf::RenderWindow& window){
}

bool GameState::shouldClose() {
	return false;
}
