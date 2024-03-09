#include "KeyState.hpp"

VirtualKeyState KeyState::m_keys[sf::Keyboard::KeyCount] = {0}, KeyState::m_mouse[sf::Mouse::ButtonCount] = { 0 };
VirtualKeyState KeyState::m_lastKeys[sf::Keyboard::KeyCount] = { 0 }, KeyState::m_lastMouse[sf::Mouse::ButtonCount] = { 0 };

sf::Vector2f KeyState::m_mousePos = { 0.0f, 0.0f };
sf::Vector2f KeyState::m_lastMousePos = { 0.0f, 0.0f };

void KeyState::update(sf::RenderWindow& window) {
	// Swap data
	std::copy(m_keys, m_keys + sf::Keyboard::KeyCount, m_lastKeys);
	std::copy(m_mouse, m_mouse + sf::Mouse::ButtonCount, m_lastMouse);
	m_lastMousePos = m_mousePos;

	// Update data
	for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
		m_keys[i].pressed = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
		m_keys[i].released = m_lastKeys[i].held && !m_keys[i].pressed;
		m_keys[i].held = m_keys[i].pressed || (m_lastKeys[i].held && !m_keys[i].released);
	}

	for (int i = 0; i < sf::Mouse::ButtonCount; ++i) {
		m_mouse[i].pressed = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i));
		m_mouse[i].released = m_lastMouse[i].held && !m_mouse[i].pressed;
		m_mouse[i].held = m_mouse[i].pressed || (m_lastMouse[i].held && !m_mouse[i].released);
	}

	m_mousePos = { 
		static_cast<float>(sf::Mouse::getPosition(window).x), 
		static_cast<float>(sf::Mouse::getPosition(window).y) 
	};
}
