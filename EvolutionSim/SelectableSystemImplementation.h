#pragma once
#include "SelectableSystemInterface.h"
#include "SelectableSystem.h"

namespace Selectable
{
	using SystemImplementation = SelectableSystem;
	using System = SelectableSystemInterface<SystemImplementation>;
};
