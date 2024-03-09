#pragma once

#include "SFML/Graphics.hpp"

struct VirtualKeyState {
	bool pressed = 0;
	bool released = 0;
	bool held = 0;
};

class KeyState {
public:
	static void update(sf::RenderWindow& window);

	static const VirtualKeyState& getKey(sf::Keyboard::Key key) { return m_keys[key]; }
	static const VirtualKeyState& getLastKey(sf::Keyboard::Key key) { return m_lastKeys[key]; }

	static const VirtualKeyState& getMouse(sf::Mouse::Button button) { return m_mouse[button]; }
	static const VirtualKeyState& getLastMouse(sf::Mouse::Button button) { return m_lastMouse[button]; }

	static const sf::Vector2f& getMousePos() { return m_mousePos; }
	static const sf::Vector2f& getLastMousePos() { return m_lastMousePos; }

private:
	KeyState() = delete;

	static VirtualKeyState m_keys[sf::Keyboard::KeyCount], m_mouse[sf::Mouse::ButtonCount];
	static VirtualKeyState m_lastKeys[sf::Keyboard::KeyCount], m_lastMouse[sf::Mouse::ButtonCount];

	static sf::Vector2f m_mousePos;
	static sf::Vector2f m_lastMousePos;
};