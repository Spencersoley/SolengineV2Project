#pragma once
#include <glm\ext\matrix_float4x4.hpp>
#include <glm\ext\matrix_transform.hpp>
struct GameData;

class CameraSystem
{
public:
	void updateCameraMatrices(GameData& gameData) const;

	void zoom(CameraComponent& component, float zoom);

	glm::vec2 convertScreenToWorld(const GameData& gameData, const glm::ivec2& screenCoords) const;
};