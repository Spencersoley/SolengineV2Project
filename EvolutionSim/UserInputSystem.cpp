#include <InputManager.h>

#include "TransformSystem.h"
#include "CameraSystem.h"
#include "SelectableSystem.h"
#include "UserInputSystem.h"

void UserInputSystem::process(const long long dt, BeingManager& beingManager, SolengineV2::GameState& gameState)
{
	inputManager.ProcessInput(gameState);

	if (inputManager.KeyState(SDLK_a))
	{
		transformSystem.translateCamera({ -0.03f * dt, 0.0f });
	}

	if (inputManager.KeyState(SDLK_d))
	{
		transformSystem.translateCamera({ 0.03f * dt, 0.0f });
	}

	if (inputManager.KeyState(SDLK_w))
	{
		transformSystem.translateCamera({ 0.0f, 0.03f * dt });
	}

	if (inputManager.KeyState(SDLK_s))
	{
		transformSystem.translateCamera({ 0.0f, -0.03f * dt });
	}

	if (inputManager.KeyState(SDLK_q))
	{
		cameraSystem.zoom(0.0005f * dt);
	}

	if (inputManager.KeyState(SDLK_e))
	{
		cameraSystem.zoom(-0.0005f * dt);
	}

	if (inputManager.KeyState(SDL_BUTTON_LEFT) && !inputManager.PreviousKeyState(SDL_BUTTON_LEFT))
	{
		selectableSystem.processClick(cameraSystem.convertScreenToWorld(inputManager.GetMouseCoords()), beingManager);
	}
}

