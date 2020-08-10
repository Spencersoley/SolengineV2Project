#pragma once
#include "CameraSystemInterface.h"
#include "CameraSystem.h"

namespace Camera
{
	using SystemImplementation = CameraSystem;
	using System = CameraSystemInterface<SystemImplementation>;
};
