#pragma once
#include "TransformSystemInterface.h"
#include "TransformSystem.h"

namespace Transform
{
	using Implementation = TransformSystem;
	using System = TransformSystemInterface<Implementation>;
};
