#pragma once
#include <glm\ext\vector_float2.hpp>

class BeingManager;
class SelectionBox;
class SelectedComponent;

class SelectableSystem
{
public:
	void processClick(const glm::vec2& mouseCoords, BeingManager& beings, SelectedTracker& selected) const;

	void update(const BeingManager& beings, SelectionBox& selectionBox, SelectedTracker& selected) const;

	void clearSelectedHandle(SelectedComponent& component) const;
};
