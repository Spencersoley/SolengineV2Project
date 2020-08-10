#pragma once
#include "CameraComponent.h"
#include "TransformComponent.h"

struct CameraManager
{
	TransformComponent transform;
	CameraComponent cam;
};