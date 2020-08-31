#pragma once
#include "TransformComponent.h"
#include "FoodComponent.h"
#include "SpriteComponent.h"

struct FoodEntity
{
	SpriteComponent sprite;
	TransformComponent transform;
	FoodComponent food;
};