#pragma once
#include "GUISystemInterface.h"
#include "GUISystem.h"

namespace GUI
{
	using SystemImplementation = GUISystem;
	using System = GUISystemInterface<SystemImplementation>;
};
