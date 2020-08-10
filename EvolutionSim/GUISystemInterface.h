#pragma once

#include <Window.h>
#include <GameState.h>

class BeingMAnager;
class DataPointManager;
class GenerationManager;
class OverlayConfig;
class SelectedTracker;

template <class Implementation>
class GUISystemInterface
{
public:
	static void init(SolengineV2::Window& wndw)
	{
		static Implementation system;
		system.init(wndw);
	}

	static void update(BeingManager& beings, DataPointManager& data, GenerationManager& generation, OverlayConfig& overlayConfig, SelectedTracker& selected, TextureLibrary& textureLibrary, SolengineV2::Window& window, SolengineV2::GameState& state, unsigned int deltaTime)
	{
		static Implementation system;
		system.update(beings, data, generation, overlayConfig, selected, textureLibrary, window, state, deltaTime);
	}
};

