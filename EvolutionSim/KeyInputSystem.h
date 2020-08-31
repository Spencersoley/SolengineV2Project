#pragma once
#include <InputManager.h>
#include <GameState.h>
#include "GameData.h"
#include "CameraSystemSpecialization.h"
#include "SelectableSystemSpecialization.h"
#include "TransformSystemSpecialization.h"
class BeingManager;
class SelectedTracker;

constexpr float TRANSLATION_CONSTANT{ 0.0002f };
constexpr float ZOOM_CONSTANT{ 0.000001f };

// UserInputSystem Specializations will cause a linker error when separating out this update function into a cpp file.
// I can't explain why. 
class KeyInputSystem
{
public:
	void update(SolengineV2::InputManager& inputManager, const std::chrono::microseconds& deltaTime, SolengineV2::GameState& gameState, GameData& gameData) const
	{
		float dt = static_cast<float>(deltaTime.count());
		inputManager.ProcessInput(gameState);

		if (inputManager.KeyState(SDLK_a))
		{
			Transform::System::translate(
				gameData.camera.transform,
				{ -TRANSLATION_CONSTANT * dt, 0.0f });
			Camera::System::setHasMoved(gameData.camera.cam, true);
		}

		if (inputManager.KeyState(SDLK_d))
		{
			Transform::System::translate(
				gameData.camera.transform, 
				{ TRANSLATION_CONSTANT * dt, 0.0f });
			Camera::System::setHasMoved(gameData.camera.cam, true);
		}

		if (inputManager.KeyState(SDLK_w))
		{
			Transform::System::translate(gameData.camera.transform, 
				{ 0.0f, TRANSLATION_CONSTANT * dt });
			Camera::System::setHasMoved(gameData.camera.cam, true);
		}

		if (inputManager.KeyState(SDLK_s))
		{
			Transform::System::translate(
				gameData.camera.transform, 
				{ 0.0f, -TRANSLATION_CONSTANT * dt });
			Camera::System::setHasMoved(gameData.camera.cam, true);
		}

		if (inputManager.KeyState(SDLK_q))
		{
			Camera::System::zoom(gameData.camera.cam, ZOOM_CONSTANT * dt);
			Camera::System::setHasMoved(gameData.camera.cam, true);
		}

		if (inputManager.KeyState(SDLK_e))
		{
			Camera::System::zoom(gameData.camera.cam, -ZOOM_CONSTANT * dt);
			Camera::System::setHasMoved(gameData.camera.cam, true);
		}

		if (inputManager.KeyState(SDL_BUTTON_LEFT) && !inputManager.PreviousKeyState(SDL_BUTTON_LEFT))
		{
			Selectable::System::processClick(Camera::System::convertScreenToWorld(gameData, inputManager.GetMouseCoords()), gameData);
		}
	}
};