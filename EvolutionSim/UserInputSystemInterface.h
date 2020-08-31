#pragma once
#include <GameState.h>
#include <InputManager.h>
struct GameData;

template <class Implementation>
class UserInputSystemInterface
{
public:
	static void update(SolengineV2::InputManager& inputManager, const std::chrono::microseconds& dt, SolengineV2::GameState& gameState, GameData& gameData)
	{
		static Implementation system;
		system.update(inputManager, dt, gameState, gameData);// worldCamera,
	}
};