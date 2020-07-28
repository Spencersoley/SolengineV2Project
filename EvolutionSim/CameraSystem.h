#pragma once
#include <glm\ext\vector_float2.hpp>
#include <glm\ext\matrix_float4x4.hpp>

class CameraComponent;
class TransformComponent;
class TransformSystem;

class CameraSystem
{
public:
	CameraSystem(
		const TransformSystem& tformSys
	);

	float            getCameraScale(const CameraComponent& component) const;
	const glm::mat4& getProjectionMatrix(const CameraComponent& component) const;

	void updateCameraMatrices(CameraComponent& camera, const TransformComponent& cameraTransform) const;

	void setHasMoved(CameraComponent& camera, bool set) const;

	void zoom(CameraComponent& camera, float zoom);

	glm::vec2 convertScreenToWorld(const CameraComponent& camera, const TransformComponent& cameraTransform, const glm::vec2& screenCoords) const;

private:
	const TransformSystem& transformSystem;
};