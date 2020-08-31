#pragma once
#include "DataSystemInterface.h"
#include "DataSystem.h"

namespace Data
{
	using SystemImplementation = DataSystem;
	using System = DataSystemInterface<SystemImplementation>;
};
