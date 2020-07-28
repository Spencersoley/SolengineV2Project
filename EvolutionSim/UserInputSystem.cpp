#include <InputManager.h>

#include "Camera.h"
#include "CameraSystem.h"
#include "SelectableSystem.h"
#include "TransformSystem.h"
#include "UserInputSystem.h"

constexpr float TRANSLATION_CONSTANT{ 0.0002f };
constexpr float ZOOM_CONSTANT	{ 0.000001f };

void UserInputSystem::update(const unsigned int dt, BeingManager& beingManager, Camera& worldCamera, SolengineV2::GameState& gameState)
{
	inputManager.ProcessInput(gameState);

	if (inputManager.KeyState(SDLK_a))
	{
		transformSystem.translate(worldCamera.transform, { -TRANSLATION_CONSTANT * dt, 0.0f });
		cameraSystem.setHasMoved(worldCamera.cam, true); 
	}

	if (inputManager.KeyState(SDLK_d))
	{
		transformSystem.translate(worldCamera.transform, { TRANSLATION_CONSTANT * dt, 0.0f });
		cameraSystem.setHasMoved(worldCamera.cam, true);
	}

	if (inputManager.KeyState(SDLK_w))
	{
		transformSystem.translate(worldCamera.transform, { 0.0f, TRANSLATION_CONSTANT * dt });
		cameraSystem.setHasMoved(worldCamera.cam, true);
	}

	if (inputManager.KeyState(SDLK_s))
	{
		transformSystem.translate(worldCamera.transform, { 0.0f, -TRANSLATION_CONSTANT * dt });
		cameraSystem.setHasMoved(worldCamera.cam, true);
	}

	if (inputManager.KeyState(SDLK_q))
	{
		cameraSystem.zoom(worldCamera.cam, ZOOM_CONSTANT * dt);
		cameraSystem.setHasMoved(worldCamera.cam, true);
	}

	if (inputManager.KeyState(SDLK_e))
	{
		cameraSystem.zoom(worldCamera.cam, -ZOOM_CONSTANT * dt);
		cameraSystem.setHasMoved(worldCamera.cam, true);
	}

	if (inputManager.KeyState(SDL_BUTTON_LEFT) && !inputManager.PreviousKeyState(SDL_BUTTON_LEFT))
	{
		selectableSystem.processClick(cameraSystem.convertScreenToWorld(worldCamera.cam, worldCamera.transform, inputManager.GetMouseCoords()), beingManager);
	}
}

