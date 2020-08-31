#pragma once
#include <glm\ext\vector_float3.hpp>

constexpr float PI = 3.1415926f;

namespace Math
{
	//makeshift constexpr ceil function- c++17 no longer needs one linr constexpr function
	static inline size_t getNOfCellsPerRow(float arenaDiameter, float cellSize) 
	{
		float adjustedDiameter = arenaDiameter + (8 * cellSize);
		float cellsPerRow = adjustedDiameter / cellSize;
		if (cellsPerRow == std::floor(cellsPerRow))
		{
			return static_cast<size_t>(cellsPerRow);
		}
		else
		{
			return static_cast<size_t>(cellsPerRow + 1);
		}
	}

	static inline glm::vec3 randomPosOutsideRadius(float minRadius, float r1, float r2)
	{
		float angle = (r1 * 2.0f - 1.0f) * PI;
		float radius = static_cast<float>(sqrt(static_cast<double>(r2) * 30.0)) + minRadius;
		return { radius * cos(angle), radius * sin(angle), 0.0f };
	}

	static inline glm::vec3 randomPosWithinRadius(float maxRadius, float r1, float r2)
	{
		float angle = (r1 * 2.0f - 1.0f) * PI;
		float radius = static_cast<float>(sqrt(r2)) * maxRadius * 0.9f;
		return { radius * cos(angle), radius * sin(angle), 0.0f };
	}
}