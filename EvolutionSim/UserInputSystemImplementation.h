#pragma once
#include "UserInputSystemInterface.h"
#include "KeyInputSystem.h"

namespace UserInput
{
	using SystemImplementation = KeyInputSystem;
	using System = UserInputSystemInterface<SystemImplementation>;
};
