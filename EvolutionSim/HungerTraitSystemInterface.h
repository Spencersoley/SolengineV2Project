#pragma once
#include <set>
#include "HungerTraitComponent.h"
#include "Handle.h"
struct GameData;
struct BeingEntity;
struct FoodEntity;

template <class Implementation>
class HungerTraitSystemInterface
{
public:
	static void setTraitValue(HungerTraitComponent& component, const float set) { component.value = set; }
	static float getTraitValue(const HungerTraitComponent& component) { return component.value; }	
	
	static void setFullness(HungerTraitComponent& component, const float set) { component.fullness = set; }
	static float getFullness(const HungerTraitComponent& component) { return component.fullness; }

	static float getMaxFullness(const HungerTraitComponent& component)
	{
		static Implementation system;
		return system.getMaxFullness(component);
	}

	static void seekNearestFood(BeingEntity& animal, const GameData& gameData)
	{
		static Implementation system;
		system.seekNearestFood(animal, gameData);
	}

	static void eatOnCollision(float foodMultiplier, Handle animalHandle, Handle foodHandle, GameData& gameData)
	{
		static Implementation system;
		system.eatOnCollision(foodMultiplier, animalHandle, foodHandle, gameData);
	}
};