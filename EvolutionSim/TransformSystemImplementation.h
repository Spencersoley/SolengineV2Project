#pragma once
#include "TransformSystemInterface.h"
#include "TransformSystem.h"

namespace Transform
{
	using SystemImplementation = TransformSystem;
	using System = TransformSystemInterface<SystemImplementation>;
};
