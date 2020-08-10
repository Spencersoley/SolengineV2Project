#pragma once
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_transform.hpp>


class TransformComponent;
class CameraComponent;

class CameraSystem
{
public:
	void updateCameraMatrices(CameraComponent& camera, const TransformComponent& cameraTransform) const;

	void zoom(CameraComponent& component, float zoom);

	glm::vec2 convertScreenToWorld(const CameraComponent& component, const TransformComponent& cameraTransform, const glm::vec2& screenCoords) const;
};
