#pragma once
#include "GeneComponent.h"
#include "SpriteComponent.h"
#include "SurvivalComponent.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"

struct Being
{
	GeneComponent gene;
	SpriteComponent sprite;
	VelocityComponent velocity;
	SurvivalComponent survival;
	TransformComponent transform;
};
