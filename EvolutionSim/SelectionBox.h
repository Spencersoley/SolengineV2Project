#pragma once
#include "ShapeComponent.h"
#include "TransformComponent.h"

struct SelectionBox
{
	TransformComponent transform;
	ShapeComponent shape;
};
