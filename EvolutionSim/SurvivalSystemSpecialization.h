#pragma once
#include "SurvivalSystemInterface.h"
#include "SurvivalSystem.h"

namespace Survival
{
	using SystemImplementation = SurvivalSystem;
	using System = SurvivalSystemInterface<SystemImplementation>;
};
