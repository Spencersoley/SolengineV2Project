#pragma once
#include "CameraComponent.h"
#include "TransformComponent.h"

struct CameraEntity
{
	TransformComponent transform;
	CameraComponent cam;
};