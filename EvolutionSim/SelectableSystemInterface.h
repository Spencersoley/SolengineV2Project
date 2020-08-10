#pragma once
#include "SelectedComponent.h"

class BeingManager;
class SelectionBox;
class SelectedTracker;

template <class Implementation>
class SelectableSystemInterface
{
	using Handle = unsigned int;
public:
	static void setHandle(SelectedComponent& selected, const Handle set) { selected.handle = set; }
	static Handle getHandle(SelectedComponent& selected) { return selected.handle; }

	static void processClick(const glm::vec2& mouseCoords, BeingManager& beings, SelectedTracker& selectedTracker)
	{
		static Implementation system;
		system.processClick(mouseCoords, beings, selectedTracker);
	}

	static void update(const BeingManager& beings, SelectionBox& selectionBox, SelectedTracker& selectedTracker)
	{
		static Implementation system;
		system.update(beings, selectionBox, selectedTracker);
	}

	static void clearSelectedHandle(SelectedComponent& selected)
	{
		static Implementation system;
		system.clearSelectedHandle(selected);
	}
};