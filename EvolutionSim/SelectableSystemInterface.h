#pragma once
#include "SelectedComponent.h"
#include "Handle.h"
struct GameData;

template <class Specialization>
class SelectableSystemInterface
{
public:
	static void setSelectedAnimalHandle(SelectedComponent& selected, const Handle set) { selected.beingHandle = set; }
	static Handle getSelectedAnimalHandle(SelectedComponent& selected) { return selected.beingHandle; }
	static void setFoodHandle(SelectedComponent& selected, const Handle set) { selected.foodHandle = set; }
	static Handle getFoodHandle(SelectedComponent& selected) { return selected.foodHandle; }

	static void processClick(const glm::vec2& mouseCoords, GameData& gameData)
	{
		static Specialization system;
		system.processClick(mouseCoords, gameData);
	}

	static void update(GameData& gameData)
	{
		static Specialization system;
		system.update(gameData);
	}

	static void clearSelectedHandle(SelectedComponent& selected)
	{
		static Specialization system;
		system.clearSelectedHandle(selected);
	}
};