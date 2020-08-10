#pragma once
#include <set>

#include "ArenaSize.h"

class BeingManager;
class DataPointManager;
class GenerationManager;
class SelectedTracker;
class TextureLibrary;

class GenerationSystem
{
	using Handle = unsigned int;

public:
	void begin(BeingManager& beings, GenerationManager& generation, SelectedTracker& selected, TextureLibrary& textureLibrary);
	void update(BeingManager& beings, DataPointManager& data, GenerationManager& generation, OverlayConfig& overlayConfig, SelectedTracker& selected, TextureLibrary& textureLibrary, std::set<Handle>& handlesToDelete);

private:
	void newWave(BeingManager& beings, DataPointManager& data, GenerationManager& generation, OverlayConfig& overlayConfig, SelectedTracker& selected, TextureLibrary& textureLibrary);
};

