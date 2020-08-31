#pragma once
#include <chrono>
#include <Window.h>
#include <GameState.h>
struct GameData;

template <class Implementation>
class GUISystemInterface
{
public:
	static void init(SolengineV2::Window& wndw)
	{
		static Implementation system;
		system.init(wndw);
	}

	static void update(SolengineV2::Window& window, SolengineV2::GameState& state, const std::chrono::microseconds& deltaTime, GameData& gameData)
	{
		static Implementation system;
		system.update(window, state, deltaTime, gameData);
	}
};