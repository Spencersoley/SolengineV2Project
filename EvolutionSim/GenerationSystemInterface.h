#pragma once
#include <vector>
#include <set>

#include "GenerationComponent.h"

class BeingManager;
class DataPointManager;
class GenerationManager;
class OverlayConfig;
class SelectedTracker;
class TextureLibrary;
using Handle = unsigned int;

template <class Implementation>
class GenerationSystemInterface
{

public:
	static int   getAnimalCount(const GenerationComponent& component) { return component.animalCount; }
	static int   getPlantCount(const GenerationComponent& component) { return component.plantCount; }
	static int   getFoodPerGeneration(const GenerationComponent& component) { return component.foodPerGeneration; }
	static int   getWaveCount(const GenerationComponent& component) { return component.waveCount; }
	static bool  getWaveOver(const GenerationComponent& component) { return component.waveOver; }
	static bool  getHuntOver(const GenerationComponent& component) { return component.huntOver; }
	static bool  getFoodHandlesNeedUpdate(const GenerationComponent& component) { return component.foodHandlesNeedUpdate; }
	static std::vector<Handle>& getFoodHandles(GenerationComponent& component) { return component.foodHandles; }

	static float& getPhysicsSpeedRef(GenerationComponent& component) { return component.physicsSpeed; } 	//maybe move this
	static float getPhysicsSpeed(const GenerationComponent& component) { return component.physicsSpeed; } 	//maybe move this
	

	static void setAnimalCount(GenerationComponent& component, const int set) { component.animalCount = set; }
	static void setPlantCount(GenerationComponent& component, const int set) { component.plantCount = set; }
	static void setFoodPerGeneration(GenerationComponent& component, const int set) { component.foodPerGeneration = set; }
	static void setWaveCount(GenerationComponent& component, const int set) { component.waveCount = set; }
	static void setWaveOver(GenerationComponent& component, const bool set) { component.waveOver = set; }
	static void setHuntOver(GenerationComponent& component, const bool set) { component.huntOver = set; }
	static void setFoodHandlesNeedUpdate(GenerationComponent& component, const bool set) { component.foodHandlesNeedUpdate = set; }


	static int& getFoodPerGenerationRef(GenerationComponent& component) { return component.foodPerGeneration; }

	static void begin(BeingManager& beings, GenerationManager& generation, SelectedTracker& selected, TextureLibrary& textureLibrary)
	{
		static Implementation system;
		system.begin(beings, generation, selected, textureLibrary);
	}

	static void update(BeingManager& beings, DataPointManager& data, GenerationManager& generation, OverlayConfig& overlayConfig, SelectedTracker& selected, TextureLibrary& textureLibrary, std::set<Handle>& handlesToDelete)
	{
		static Implementation system;
		system.update(beings, data, generation, overlayConfig, selected, textureLibrary, handlesToDelete);
	}
};

