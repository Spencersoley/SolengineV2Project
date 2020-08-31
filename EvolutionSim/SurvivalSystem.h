#pragma once
#include <vector>
#include <glm\ext\vector_float2.hpp>
#include <set>
#include <chrono>
#include "Handle.h"
struct GameData;
struct BeingEntity;
struct FoodEntity;

class SurvivalSystem
{
	using SurvivalState = SurvivalComponent::SurvivalState;
public:
	void update(const std::chrono::microseconds& dt, GameData& gameData);

private:
	enum class WaveState
	{
		NOTHING_ACTIVE = 1 << 0,
		SEARCHERS_ACTIVE = 1 << 1,
		RETURNERS_ACTIVE = 1 << 2,
	};
};