#pragma once
#include "SpriteSystemInterface.h"
#include "SpriteSystem.h"

namespace Sprite
{
	using SystemImplementation = SpriteSystem;
	using System = SpriteSystemInterface<SystemImplementation>;
};
