#pragma once
#include <glm\ext\matrix_float4x4.hpp>

constexpr float DEFAULT_CAMERA_ZOOM = 0.2f;

class CameraComponent
{
	friend class CameraSystem;
	friend struct Camera;

	glm::mat4 projectionMatrix{ 0.0f };
	float scale{ DEFAULT_CAMERA_ZOOM };
	bool hasMoved = true;
};
