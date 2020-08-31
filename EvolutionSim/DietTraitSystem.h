#pragma once
struct FoodEntity;
struct BeingEntity;

class DietTraitSystem
{
public:
	float getDietaryBonus(FoodEntity& food, BeingEntity& being) const;
};