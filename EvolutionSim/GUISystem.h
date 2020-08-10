#pragma once
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_sdl.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include <IMGUI/implot.h>
#include <numeric>
#include <Window.h>
#include <GameState.h>

namespace SolengineV2
{
	class Window;
}

class BeingManager;
class DataPointManager;
class GenerationManager;
class OverlayConfig;
class SelectedTracker;
class TextureLibrary;

class GUISystem
{
	using Handle = unsigned int;

public:
	void init(SolengineV2::Window& wndw);

	void update(BeingManager& beings, DataPointManager& data, GenerationManager& generation, OverlayConfig& overlayConfig, SelectedTracker& selected, TextureLibrary& textureLibrary, SolengineV2::Window& window, SolengineV2::GameState& state, unsigned int deltaTime);

private:
	void resetGame(SolengineV2::GameState& state, BeingManager& beings, GenerationManager& generation, DataPointManager& data, SelectedTracker& selected, TextureLibrary& textureLibrary);

	enum class Menu { SETTINGS, HELP, OVERLAY };
	Menu menu{ Menu::SETTINGS };
};

