#pragma once
#include "GeneSystemInterface.h"
#include "GeneSystem.h"

namespace Gene
{
	using SystemImplementation = GeneSystem;
	using System = GeneSystemInterface<SystemImplementation>;
};
