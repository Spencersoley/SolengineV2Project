#include "PartitionSystemImplementation.h"
#include "TransformSystemImplementation.h"

#include "BeingManager.h"
#include "ArenaSize.h"

void SpatialPartitionSystem::update(BeingManager& beings, PartitionComponent& component)
{
	Grid& grid = Partition::System::getGrid(component);
	for (size_t i = 0; i < CELL_COUNT; ++i)
	{
		for (size_t j = 0; j < CELL_COUNT; ++j)
		{
			grid[i][j].clear();
		}
	}

	for (Handle beingHandle = 0; beingHandle < beings.getSize(); beingHandle++)
	{
		const glm::vec2& pos = Transform::System::getPos(beings.pool[beingHandle].transform);

		int x = static_cast<int>((pos.x / CELL_SIZE) + (CELL_COUNT / 2));
		x = std::min(std::max(0, x), static_cast<int>(CELL_COUNT - 1));
		int y = static_cast<int>((pos.y / CELL_SIZE) + (CELL_COUNT / 2));
		y = std::min(std::max(0, y), static_cast<int>(CELL_COUNT - 1));

		grid[x][y].push_back(beingHandle);
	}
}

void SpatialPartitionSystem::setupGrid(PartitionComponent& component)
{
	Partition::System::setGrid(component, Grid(CELL_COUNT, Row(CELL_COUNT, Cell{})));
}

