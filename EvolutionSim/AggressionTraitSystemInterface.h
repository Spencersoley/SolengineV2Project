#pragma once
#include "AggressionTraitComponent.h"
#include "Handle.h"
struct GameData;

template <class Implementation>
class AggressionTraitSystemInterface
{
public:
	static void setTraitValue(AggressionTraitComponent& component, const float set) { component.value = set; }
	static float getTraitValue(const AggressionTraitComponent& component) { return component.value; }

	static void resolveAggressionTest(Handle animalHandleA, Handle animalHandleB, float combatMultiplier, GameData& gameData)
	{
		static Implementation system;
		system.resolveAggressionTest(animalHandleA, animalHandleB, combatMultiplier, gameData);
	}
};