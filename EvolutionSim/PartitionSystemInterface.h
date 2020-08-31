#pragma once
#include "Grid.h"
#include "PartitionComponent.h"
struct GameData;

template <class Implementation>
class PartitionSystemInterface
{
public:
	static Grid& getAnimalGrid(PartitionComponent& component) { return component.animalGrid; }
	static Grid& getFoodGrid(PartitionComponent& component) { return component.foodGrid; }

	static void setAnimalGrid(PartitionComponent& component, Grid grid) 
	{
		component.animalGrid = grid; 
	}	
	
	static void setFoodGrid(PartitionComponent& component, Grid grid) 
	{
		component.foodGrid = grid; 
	}

	static void update(GameData& gameData)
	{
		static Implementation system;
		system.update(gameData);
	}
	
	static void setupGrid(PartitionComponent& component, GameData& gameData)
	{
		static Implementation system;
		system.setupGrid(component, gameData);
	}
};
