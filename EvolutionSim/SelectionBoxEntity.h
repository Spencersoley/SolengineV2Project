#pragma once
#include "ShapeComponent.h"
#include "TransformComponent.h"

struct SelectionBoxEntity
{
	TransformComponent transform;
	ShapeComponent shape;
};