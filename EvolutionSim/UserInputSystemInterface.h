#pragma once
#include <GameState.h>
#include <InputManager.h>

class BeingManager;
class CameraManager;
class SelectedTracker;

template <class Implementation>
class UserInputSystemInterface
{
public:
	static void update(SolengineV2::InputManager& inputManager, const unsigned int dt, BeingManager& beingManager, CameraManager& worldCamera, SelectedTracker& selected, SolengineV2::GameState& gameState)
	{
		static Implementation system;
		system.update(inputManager, dt, beingManager, gameState, worldCamera, selected);// worldCamera,
	}
};