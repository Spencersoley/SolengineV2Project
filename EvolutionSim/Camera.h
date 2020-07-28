#pragma once
#include "CameraComponent.h"
#include "TransformComponent.h"

struct Camera
{
	TransformComponent transform;
	CameraComponent cam;
};