#pragma once
#include "ShapeSystemInterface.h"
#include "ShapeSystem.h"

namespace Shape
{
	using SystemImplementation = ShapeSystem;
	using System = ShapeSystemInterface<SystemImplementation>;
};
