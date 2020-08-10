#pragma once

constexpr float ARENA_SIZE = 1600.0f;

constexpr size_t CELL_SIZE{ 40 };
constexpr float GRID_SIZE{ ARENA_SIZE + (2 * CELL_SIZE) };
//makeshift constexpr ceil function- c++17 no longer needs one linr constexpr function
constexpr size_t CELL_COUNTER() noexcept
{
	float cell = GRID_SIZE / CELL_SIZE;
	if (cell == static_cast<float>(static_cast<unsigned int>(cell)))
	{
		return static_cast<unsigned int>(cell);
	}
	else
	{
		return static_cast<unsigned int>(cell + 1);
	}
}

constexpr size_t CELL_COUNT = CELL_COUNTER();