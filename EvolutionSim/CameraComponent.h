#pragma once
#include <glm\ext\matrix_float4x4.hpp>

class CameraComponent
{
	template <typename T> friend class CameraSystemInterface;

	glm::mat4 projectionMatrix{ 0.0f };
	float scale{ 0.2f };
	bool hasMoved = true;
};
