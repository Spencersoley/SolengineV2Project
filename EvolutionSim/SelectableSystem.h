#pragma once
#include <glm\ext\vector_float2.hpp>
struct GameData;

class SelectableSystem
{
public:
	void processClick(const glm::vec2& mouseCoords, GameData& gameData) const;

	void update(GameData& gameData) const;

	void clearSelectedHandle(SelectedComponent& component) const;
};