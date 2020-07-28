#include <glm\ext\matrix_clip_space.hpp>
#include <glm\ext\matrix_transform.hpp>

#include "Sceensize.h"
#include "CameraSystem.h"
#include "TransformSystem.h"

#include "CameraComponent.h"
#include <algorithm>

constexpr float MAX_SCALE{ 10.0f };
constexpr float MIN_SCALE{ 0.2f };
//this should be generated with a function if we intend to change screen dimensions
constexpr glm::mat4 ORTHO_MATRIX = glm::mat4
{
	{ 2.0f / static_cast<float>(SCREEN_WIDTH), 0.0f,                                      0.0f, 0.0f },
	{ 0.0f,                                    2.0f / static_cast<float>(SCREEN_HEIGHT),  0.0f, 0.0f },
	{ 0.0f,                                    0.0f,                                     -1.0f, 0.0f },
	{-1.0f,                                   -1.0f,                                      0.0f, 1.0f },
};

CameraSystem::CameraSystem(
	const TransformSystem& tformSys
) :
	transformSystem(tformSys)
{}

float CameraSystem::getCameraScale(const CameraComponent & component) const
{
	return component.scale;
}

const glm::mat4& CameraSystem::getProjectionMatrix(const CameraComponent& component) const
{
	return component.projectionMatrix; 
}

void CameraSystem::updateCameraMatrices(CameraComponent& camera, const TransformComponent& cameraTransform) const
{
	const glm::vec2& currentCameraPosition = transformSystem.getPos(cameraTransform);
	if (camera.hasMoved)
	{
		const glm::vec3 translationVec = { ((float)SCREEN_WIDTH / 2.0f) - currentCameraPosition.x, ((float)SCREEN_HEIGHT / 2.0f) - currentCameraPosition.y, 0.0f };
		const glm::mat4 pMatrix = glm::translate(ORTHO_MATRIX, translationVec);
		const glm::vec3 scaleVec(camera.scale, camera.scale, 1.0f);
		camera.projectionMatrix = glm::scale(glm::mat4(1.0f), scaleVec) * pMatrix;
		camera.hasMoved = false;
	}
}

void CameraSystem::setHasMoved(CameraComponent& camera, bool set) const
{
	camera.hasMoved = set;
}

void CameraSystem::zoom(CameraComponent& camera, float zoom)
{
	camera.scale = std::min(MAX_SCALE, camera.scale + zoom);
	camera.scale = std::max(MIN_SCALE, camera.scale);
}

glm::vec2 CameraSystem::convertScreenToWorld(const CameraComponent& camera, const TransformComponent& cameraTransform, const glm::vec2& screenCoords) const
{
	glm::vec2 coords = screenCoords;
	//Invert Y direction
	coords.y = SCREEN_HEIGHT - coords.y;
	//Converts 0 to centre
	coords -= glm::vec2{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	//Scale the coordinates
	coords /= camera.scale;
	//Translate with camera;
	coords += transformSystem.getPos(cameraTransform);
	return coords;
}
