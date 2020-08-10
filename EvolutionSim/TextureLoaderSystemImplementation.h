#pragma once
#include "TextureLoaderSystemInterface.h"
#include "TextureSystem.h"

namespace TextureLoader
{
	using SystemImplementation = TextureSystem;
	using System = TextureLoaderSystemInterface<SystemImplementation>;
};
