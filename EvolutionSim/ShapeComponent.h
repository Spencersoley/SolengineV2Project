#pragma once
#include <GeometryRenderer.h>

class ShapeComponent
{
	friend class ShapeSystem;
	friend struct SelectionBox;

	SolengineV2::Square shape;
};
