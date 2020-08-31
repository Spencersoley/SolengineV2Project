#pragma once
#include <glm\ext\vector_float2.hpp>
#include <tuple>
#include "Handle.h"
#include "StrengthTraitComponent.h"
struct GameData;

template <class Implementation>
class StrengthTraitSystemInterface
{
public:
	static void setTraitValue(StrengthTraitComponent& component, const float set) { component.value = set; }
	static float getTraitValue(const StrengthTraitComponent& component) { return component.value; }

	static void resolveStrengthCollision(Handle animalHandleA, Handle animalHandleB, GameData& gameData, glm::vec2& collisionDepthVec)
	{
		static Implementation system;
		system.resolveStrengthCollision(animalHandleA, animalHandleB, gameData, collisionDepthVec);
	}

	static std::tuple<float, float> getCombatBonus(Handle animalHandleA, Handle animalHandleB, GameData& gameData)
	{
		static Implementation system;
		return system.getCombatBonus(animalHandleA, animalHandleB, gameData);
	}
};