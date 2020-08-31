#pragma once
#include <glm\ext\vector_float2.hpp>
#include <tuple>
#include "Handle.h"
struct GameData;

class StrengthTraitSystem
{
public:
	void resolveStrengthCollision(Handle animalHandleA, Handle animalHandleB, GameData& gameData, glm::vec2& collisionDepthVec);

	std::tuple<float, float> getCombatBonus(Handle animalHandleA, Handle animalHandleB, GameData& gameData);
};

