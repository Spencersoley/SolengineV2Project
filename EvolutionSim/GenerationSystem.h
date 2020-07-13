#pragma once
#include <cstdint>

constexpr int INITIAL_FOOD_COUNT = 25;

class DataSystem;
class SelectableSystem;
class SurvivalSystem;
class BeingManager;
class GeneSystem;

class GenerationSystem
{
public:
	GenerationSystem(
		SelectableSystem& selSys,
		SurvivalSystem& survSys,
		DataSystem& dataSys,
		const GeneSystem& geneSys
	) :
		selectableSystem(selSys),
		survivalSystem(survSys),
		dataSystem(dataSys),
		geneSystem(geneSys)
	{}

	int& getFoodPerGeneration() { return foodPerGeneration; } //int& nonconst for ImGui slider
	uint32_t getWaveCount() const { return waveCount; }
	uint32_t getAnimalCount() const { return animalCount; }
	uint32_t getPlantCount() const { return plantCount; }

	void begin(BeingManager& beings);
	void process(BeingManager& beings);

private:
	SelectableSystem& selectableSystem;
	SurvivalSystem& survivalSystem;
	DataSystem& dataSystem;
	const GeneSystem& geneSystem;

	int foodPerGeneration{ INITIAL_FOOD_COUNT };
	uint32_t waveCount{ 1 };
	uint32_t animalCount{ 0 };
	uint32_t plantCount{ 0 };

	float totalSpeed{ 0.0f };
	float totalStrength{ 0.0f };
	float totalDiet{ 0.0f };
	float totalStamina{ 0.0f };
	int totalActiveBeings{ 0 };

	void newWave(BeingManager& beings);
};
