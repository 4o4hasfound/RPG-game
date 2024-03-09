#pragma once
#include <string>
#include <vector>

#include "State.hpp"
#include "States/ShowTextState.hpp"
#include "States/MainGameState.hpp"
#include "States/ResetGameState.hpp"

#include "Physics/Body.hpp"
#include "Physics/AABB.hpp"

#include "Character.hpp"

class WinArea : public Body {
public:
	WinArea(const AABB& aabb, StateManager& manager, const std::string& text);
	WinArea(const AABB& aabb, StateManager& manager, const std::vector<std::string>& text);
private:
	virtual void onCollide(Body* other, const CollisionDetail& detail) override final;
	virtual void afterCollisionTestCallback() override final;

	bool m_opened = 0;
	std::vector<std::string> m_texts;
	StateManager& m_stateManager;
};