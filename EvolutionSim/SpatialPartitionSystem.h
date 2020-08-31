#pragma once
#include "Grid.h"
struct GameData;

class SpatialPartitionSystem
{
public:
	void update(GameData& gameData) const;
	void setupGrid(PartitionComponent& component, GameData& gameData) const;
};