#pragma once
#include <vector>
#include <set>
#include <chrono>
#include "GenerationComponent.h"
#include "Handle.h"
struct GameData;
namespace SolengineV2
{
	class RandomNumberGenerator;
}

template <class Implementation>
class GenerationSystemInterface
{
public:
	static int   getAnimalCount(const GenerationComponent& component) { return component.animalCount; }
	static int   getPlantCount(const GenerationComponent& component) { return component.plantCount; }
	static int   getMeatCount(const GenerationComponent& component) { return component.meatCount; }
	static int   getToxicCount(const GenerationComponent& component) { return component.toxicCount; }
	static int   getWaveCount(const GenerationComponent& component) { return component.waveCount; }
	static bool  getWaveOver(const GenerationComponent& component) { return component.waveOver; }
	static bool  getHuntOver(const GenerationComponent& component) { return component.huntOver; }

	static void setAnimalCount(GenerationComponent& component, const int set) { component.animalCount = set; }
	static void setPlantCount(GenerationComponent& component, const int set) { component.plantCount = set; }
	static void setMeatCount(GenerationComponent& component, const int set) { component.meatCount = set; }
	static void setToxicCount(GenerationComponent& component, const int set) { component.toxicCount = set; }
	static void setWaveCount(GenerationComponent& component, const int set) { component.waveCount = set; }
	static void setWaveOver(GenerationComponent& component, const bool set) { component.waveOver = set; }
	static void setHuntOver(GenerationComponent& component, const bool set) { component.huntOver = set; }

	static void begin(GameData& gameData, SolengineV2::RandomNumberGenerator& rng)
	{
		static Implementation system;
		system.begin(gameData, rng);
	}

	static void update(GameData& gameData, SolengineV2::RandomNumberGenerator& rng, const std::chrono::microseconds& deltaTime)
	{
		static Implementation system;
		system.update(gameData, rng, deltaTime);
	}
};

