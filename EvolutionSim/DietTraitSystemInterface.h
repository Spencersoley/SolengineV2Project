#pragma once
#include "DietTraitComponent.h"
struct GameData;
struct FoodEntity;
struct BeingEntity;

template <class Implementation>
class DietTraitSystemInterface
{
public:
	static void setTraitValue(DietTraitComponent& component, const float set) { component.value = set; }
	static float getTraitValue(const DietTraitComponent& component) { return component.value; }

	static float getDietaryBonus(FoodEntity& food, BeingEntity& being)
	{
		static Implementation system;
		return system.getDietaryBonus(food, being);
	}
};