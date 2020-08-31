#include <algorithm>
#include "Sceensize.h"
#include "TransformSystemSpecialization.h"
#include "CameraSystemSpecialization.h"
#include "GameData.h"

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

void CameraSystem::zoom(CameraComponent& component, float zoom)
{
	float scale = Camera::System::getScale(component);
	scale = std::min(MAX_SCALE, scale + zoom);
	scale = std::max(MIN_SCALE, scale);
	Camera::System::setScale(component, scale);
}

void CameraSystem::updateCameraMatrices(GameData& gameData) const
{
	const glm::vec2& currentCameraPosition = Transform::System::getPos(gameData.camera.transform);
	if (Camera::System::getHasMoved(gameData.camera.cam))
	{
		const glm::vec3 translationVec = { ((float)SCREEN_WIDTH / 2.0f) - currentCameraPosition.x, ((float)SCREEN_HEIGHT / 2.0f) - currentCameraPosition.y, 0.0f };
		const glm::mat4 pMatrix = glm::translate(ORTHO_MATRIX, translationVec);
		float cameraScale = Camera::System::getScale(gameData.camera.cam);
		const glm::vec3 scaleVec(cameraScale, cameraScale, 1.0f);
		Camera::System::setProjectionMatrix(gameData.camera.cam, (glm::scale(glm::mat4(1.0f), scaleVec) * pMatrix));
		Camera::System::setHasMoved(gameData.camera.cam, false);
	}
}

glm::vec2 CameraSystem::convertScreenToWorld(const GameData& gameData, const glm::ivec2& screenCoords) const
{
	glm::vec2 coords = screenCoords;
	//Invert Y direction
	coords.y = SCREEN_HEIGHT - coords.y;
	//Converts 0 to centre
	coords -= glm::vec2{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	//Scale the coordinates
	coords /= Camera::System::getScale(gameData.camera.cam);
	//Translate with camera;
	coords += Transform::System::getPos(gameData.camera.transform);
	return coords;
}