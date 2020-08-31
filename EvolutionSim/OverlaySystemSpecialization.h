#pragma once
#include "OverlaySystemInterface.h"
#include "OverlaySystem.h"

namespace Overlay
{
	using SystemImplementation = OverlaySystem;
	using System = OverlaySystemInterface<SystemImplementation>;
};
