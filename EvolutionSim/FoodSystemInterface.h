#pragma once
#include "FoodComponent.h"

//interface currently has no implementation details therefore needs no implementation
class FoodSystem
{
public:
	static void setSaturation(FoodComponent& food, float set) { food.saturation = set; }
	static float getSaturation(const FoodComponent& food) { return food.saturation; }	
	
	static void setIsToxic(FoodComponent& food, float set) { food.isToxic = set; }
	static bool getIsToxic(const FoodComponent& food) { return food.isToxic; }

	static FoodType getFoodType(const FoodComponent& food) { return food.type; }
	static void setFoodType(FoodComponent& food, FoodType set) { food.type = set; }

	static void depleteSaturation(FoodComponent& food, float modify) { food.saturation = std::max(food.saturation - modify, 0.0f); }
};
