#include "Objects/TextPopup.hpp"

TextPopup::TextPopup(const AABB& aabb, StateManager& manager, const std::string& text)
	: Body(BodyType::Static_Uncollidable, aabb), m_stateManager(manager), m_texts({text}) {

}

TextPopup::TextPopup(const AABB& aabb, StateManager& manager, const std::vector<std::string>& text)
	: Body(BodyType::Static_Uncollidable, aabb), m_stateManager(manager), m_texts(text) {

}

void TextPopup::onCollide(Body* other, const CollisionDetail& detail) {
	if (other->getType() != BodyType::Character) {
		return;
	}
	if (m_opened) {
		return;
	}
	m_opened = 1;
	MainGameState* state = dynamic_cast<MainGameState*>(m_stateManager.topState());
	for (int i = m_texts.size() - 1; i >= 0; --i) {
		m_stateManager.pushState(new ShowTextState(
			m_stateManager,
			state->getFrame().getTexture(),
			sf::Vector2f{
				static_cast<float>(state->getFrame().getSize().x) * 0.5f,
				static_cast<float>(state->getFrame().getSize().y) * 0.5f
			},
			m_texts[i],
			50.0f
		));
	}
}

void TextPopup::afterCollisionTestCallback() {

}
