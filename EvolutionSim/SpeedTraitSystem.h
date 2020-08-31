#pragma once
#include <glm\ext\vector_float2.hpp>
#include <chrono>
struct GameData;

class SpeedTraitSystem
{
public:
	void update(const std::chrono::microseconds& dt, GameData& gameData) const;
};