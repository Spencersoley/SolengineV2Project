#pragma once
#include "GenerationSystemInterface.h"
#include "GenerationSystem.h"

namespace Generation
{
	using SystemImplementation = GenerationSystem;
	using System = GenerationSystemInterface<SystemImplementation>;
};
