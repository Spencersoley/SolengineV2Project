#pragma once
#include <map>
#include <vector>

#include "TransformSystem.h"

constexpr uint32_t CELL_SIZE{ 40 };
constexpr float GRID_SIZE{ ARENA_SIZE + (2 * CELL_SIZE) };
//makeshift constexpr ceil function- c++17 no longer needs one linr constexpr function
constexpr uint32_t CELL_COUNTER() noexcept
{
	float cell = GRID_SIZE / CELL_SIZE;
	if (cell == static_cast<float>(static_cast<uint32_t>(cell)))
	{
		return static_cast<uint32_t>(cell);
	}
	else
	{
		return static_cast<uint32_t>(cell + 1);
	}
}

constexpr uint32_t CELL_COUNT = CELL_COUNTER();

using Cell = std::vector<uint32_t>;
using Row = std::vector<Cell>;
using Grid = std::vector<Row>;

class BeingManager;

class PartitionSystem
{
public:
	PartitionSystem(
		const TransformSystem& tformSys
	) :
		transformSystem(tformSys)
	{}

	Grid processBeingGrid(const BeingManager& beings) const;

	std::vector<uint32_t> processFoodHandles(const BeingManager& beings) const;

private:
	const TransformSystem& transformSystem;

	Grid beingGrid = Grid(CELL_COUNT, Row(CELL_COUNT, Cell{}));

	std::vector<uint32_t> foodHandles{};
};
