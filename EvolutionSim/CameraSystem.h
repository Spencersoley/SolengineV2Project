#pragma once

class TransformSystem;

class CameraSystem
{
public:
	CameraSystem(
		const TransformSystem& tformSys
	);

	float            getCameraScale() const { return scale; }
	const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
	const glm::mat4& getUIMatrix() const { return uiMatrix; }

	void updateCameraMatrices();

	void zoom(float zoom);

	glm::vec2 convertScreenToWorld(const glm::vec2& screenCoords) const;

private:
	const TransformSystem& transformSystem;

	glm::mat4 orthoMatrix{};
	glm::mat4 projectionMatrix{ 0.0f };
	glm::mat4 uiMatrix{};
	float scale{ 0.6f };
	bool cameraHasZoomed = true;
	glm::vec2 previousCameraPosition{};
};