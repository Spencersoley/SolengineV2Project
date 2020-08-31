#pragma once
#include "Handle.h"
struct BeingEntity;
struct FoodEntity;
struct HungerTraitComponent;
struct GameData;

class HungerTraitSystem
{
public:
	float getMaxFullness(const HungerTraitComponent& component) const;
	void seekNearestFood(BeingEntity& animal, const GameData& gameData) const;
	void eatOnCollision(float foodMultiplier, Handle animalHandle, Handle foodHandle, GameData& gameData) const;
};