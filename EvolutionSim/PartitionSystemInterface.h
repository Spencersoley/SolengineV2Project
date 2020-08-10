#pragma once
#include "Grid.h"
#include "PartitionComponent.h"

class BeingManager;

template <class Implementation>
class PartitionSystemInterface
{
public:
	static Grid& getGrid(PartitionComponent& component) { return component.grid; }

	static void setGrid(PartitionComponent& component, Grid&& grid) { component.grid = grid; }

	static void update(BeingManager& beings, PartitionComponent& component)
	{
		static Implementation system;
		system.update(beings, component);
	}
	
	static void setupGrid(PartitionComponent& component)
	{
		static Implementation system;
		system.setupGrid(component);
	}
};
