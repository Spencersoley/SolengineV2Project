#pragma once
#include "Grid.h"

class BeingManager;
class PartitionComponent;

class SpatialPartitionSystem
{
public:
	void update(BeingManager& beings, PartitionComponent& grid);

	void setupGrid(PartitionComponent& grid);
};
