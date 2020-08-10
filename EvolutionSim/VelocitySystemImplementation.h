#pragma once
#include "VelocitySystemInterface.h"
#include "VelocitySystem.h"

namespace Velocity
{
	using SystemImplementation = VelocitySystem;
	using System = VelocitySystemInterface<SystemImplementation>;
};
