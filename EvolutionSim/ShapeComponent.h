#pragma once
#include <GeometryRenderer.h>

class ShapeComponent
{
	template <class T> friend class ShapeSystemInterface;

	SolengineV2::Square square;
};
