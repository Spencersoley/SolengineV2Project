#pragma once
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_sdl.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include <IMGUI/implot.h>
#include <numeric>
#include <chrono>
#include <Window.h>
#include <GameState.h>
#include "Handle.h"
namespace SolengineV2
{
	class Window;
}
struct GameData;

class GUISystem
{
public:
	void init(SolengineV2::Window& wndw);

	void update(SolengineV2::Window& window, SolengineV2::GameState& state, const std::chrono::microseconds& deltaTime, GameData& gameData);

private:
	void resetGame(SolengineV2::GameState& state);

	enum class Menu { SETTINGS, INFO };
	Menu menu{ Menu::INFO };
};