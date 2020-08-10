#pragma once
#include <InputManager.h>
#include <GameState.h>

#include "CameraManager.h"

#include "CameraSystemImplementation.h"
#include "SelectableSystemImplementation.h"
#include "TransformSystemImplementation.h"

class BeingManager;
class SelectedTracker;

constexpr float TRANSLATION_CONSTANT{ 0.0002f };
constexpr float ZOOM_CONSTANT{ 0.000001f };

// UserInputSystem implementations will cause a linker error when separating out this update function into a cpp file.
// I can't explain why. 
class KeyInputSystem
{
public:
	void update(SolengineV2::InputManager& inputManager, const unsigned int dt, BeingManager& beingManager, SolengineV2::GameState& gameState, CameraManager& worldCamera, SelectedTracker& selected)
	{
		inputManager.ProcessInput(gameState);

		if (inputManager.KeyState(SDLK_a))
		{
			Transform::System::translate(worldCamera.transform, { -TRANSLATION_CONSTANT * dt, 0.0f });
			Camera::System::setHasMoved(worldCamera.cam, true);
		}

		if (inputManager.KeyState(SDLK_d))
		{
			Transform::System::translate(worldCamera.transform, { TRANSLATION_CONSTANT * dt, 0.0f });
			Camera::System::setHasMoved(worldCamera.cam, true);
		}

		if (inputManager.KeyState(SDLK_w))
		{
			Transform::System::translate(worldCamera.transform, { 0.0f, TRANSLATION_CONSTANT * dt });
			Camera::System::setHasMoved(worldCamera.cam, true);
		}

		if (inputManager.KeyState(SDLK_s))
		{
			Transform::System::translate(worldCamera.transform, { 0.0f, -TRANSLATION_CONSTANT * dt });
			Camera::System::setHasMoved(worldCamera.cam, true);
		}

		if (inputManager.KeyState(SDLK_q))
		{
			Camera::System::zoom(worldCamera.cam, ZOOM_CONSTANT * dt);
			Camera::System::setHasMoved(worldCamera.cam, true);
		}

		if (inputManager.KeyState(SDLK_e))
		{
			Camera::System::zoom(worldCamera.cam, -ZOOM_CONSTANT * dt);
			Camera::System::setHasMoved(worldCamera.cam, true);
		}

		if (inputManager.KeyState(SDL_BUTTON_LEFT) && !inputManager.PreviousKeyState(SDL_BUTTON_LEFT))
		{
			Selectable::System::processClick(Camera::System::convertScreenToWorld(worldCamera.cam, worldCamera.transform, inputManager.GetMouseCoords()), beingManager, selected);
		}
	}
};

