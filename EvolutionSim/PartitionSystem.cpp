#include <map>
#include <vector>

#include "BeingManager.h"
#include "PartitionSystem.h"
#include "TransformSystem.h"

using Handle = unsigned int;
using Cell = std::vector<Handle>;
using Row = std::vector<Cell>;
using Grid = std::vector<Row>;

//processes a being grid
const Grid& PartitionSystem::partitionBeingsToGrid(const BeingManager& beings)
{
	for (size_t i = 0; i < CELL_COUNT; ++i)
	{
		for (size_t j = 0; j < CELL_COUNT; ++j)
		{
			grid[i][j].clear();
		}
	}

	for (Handle beingHandle = 0; beingHandle < beings.getSize(); beingHandle++)
	{
		const glm::vec2& pos = transformSystem.getPos(beings.pool[beingHandle].transform);

		int x = static_cast<int>((pos.x / CELL_SIZE) + (CELL_COUNT / 2));
		x = std::min(std::max(0, x), static_cast<int>(CELL_COUNT - 1));
		int y = static_cast<int>((pos.y / CELL_SIZE) + (CELL_COUNT / 2));
		y = std::min(std::max(0, y), static_cast<int>(CELL_COUNT - 1));

		grid[x][y].push_back(beingHandle);
	}

	return grid;
}

