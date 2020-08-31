#pragma once
#include "GeneComponent.h"
#include "SpriteComponent.h"
#include "SurvivalComponent.h"
#include "TransformComponent.h"

struct BeingEntity
{
	GeneComponent gene;
	SpriteComponent sprite;
	SurvivalComponent survival;
	TransformComponent transform;
};
