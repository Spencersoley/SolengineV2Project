#include <glm\ext\matrix_clip_space.hpp>
#include <glm\ext\matrix_transform.hpp>

#include "Sceensize.h"
#include "CameraSystem.h"
#include "TransformSystem.h"

constexpr float MAX_SCALE{ 10.0f };
constexpr float MIN_SCALE{ 0.6f };

CameraSystem::CameraSystem(
	const TransformSystem& tformSys
) :
	transformSystem(tformSys)
{
	orthoMatrix = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT);
	uiMatrix = glm::translate(orthoMatrix, glm::vec3{ ((float)SCREEN_WIDTH / 2.0f), ((float)SCREEN_HEIGHT / 2.0f), 0.0f });
}

void CameraSystem::updateCameraMatrices()
{
	const glm::vec2& currentCameraPosition = transformSystem.getCameraPos();
	if (cameraHasZoomed || currentCameraPosition != previousCameraPosition)
	{
		const glm::vec3 translationVec = { ((float)SCREEN_WIDTH / 2.0f) - currentCameraPosition.x, ((float)SCREEN_HEIGHT / 2.0f) - currentCameraPosition.y, 0.0f };
		const glm::mat4 pMatrix = glm::translate(orthoMatrix, translationVec);
		const glm::vec3 scaleVec(scale, scale, 1.0f);
		projectionMatrix = glm::scale(glm::mat4(1.0f), scaleVec) * pMatrix;
		cameraHasZoomed = false;
		previousCameraPosition = currentCameraPosition;
	}
}

void CameraSystem::zoom(float zoom)
{
	scale = std::min(MAX_SCALE, scale + zoom);
	scale = std::max(MIN_SCALE, scale);
	cameraHasZoomed = true;
}

glm::vec2 CameraSystem::convertScreenToWorld(const glm::vec2& screenCoords) const
{
	glm::vec2 coords = screenCoords;
	//Invert Y direction
	coords.y = SCREEN_HEIGHT - coords.y;
	//Converts 0 to centre
	coords -= glm::vec2{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	//Scale the coordinates
	coords /= scale;
	//Translate with camera;
	coords += transformSystem.getCameraPos();
	return coords;
}
