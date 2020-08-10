#pragma once
#include "ColliderSystemInterface.h"
#include "ColliderSystem.h"

namespace Collider
{
	using SystemImplementation = ColliderSystem;
	using System = ColliderSystemInterface<SystemImplementation>;
};
