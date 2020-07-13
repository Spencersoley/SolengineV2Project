#include <map>
#include <vector>

#include "BeingManager.h"
#include "TransformSystem.h"
#include "PartitionSystem.h"

//processes a being grid
Grid PartitionSystem::processBeingGrid(const BeingManager& beings) const
{
	Grid grid = Grid(CELL_COUNT, Row(CELL_COUNT, Cell{}));

	for (uint32_t beingHandle = 0; beingHandle < beings.getSize(); beingHandle++)
	{
		const glm::vec2& pos = transformSystem.getPos(beings.getTransformComponent(beingHandle));

		int x = (pos.x / CELL_SIZE) + (CELL_COUNT / 2);
		x = std::max(0, x);
		x = std::min(x, static_cast<int>(CELL_COUNT - 1));
		int y = (pos.y / CELL_SIZE) + (CELL_COUNT / 2);
		y = std::max(0, y);
		y = std::min(y, static_cast<int>(CELL_COUNT - 1));

		grid[x][y].push_back(beingHandle);
	}
	return grid;
}

std::vector<uint32_t> PartitionSystem::processFoodHandles(const BeingManager& beings) const
{
	std::vector<uint32_t> foodHandles;
	for (uint32_t beingHandle = 0; beingHandle < beings.getSize(); beingHandle++)
	{
		if (!beings.getSurvivalComponent(beingHandle).isAlive)
		{
			foodHandles.push_back(beingHandle);
		}
	}
	return foodHandles;
}

