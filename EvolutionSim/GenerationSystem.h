#pragma once

constexpr int INITIAL_FOOD_COUNT = 25;

class BeingManager;
class ColliderSystem;
class DataSystem;
class GeneSystem;
class OverlaySystem;
class SelectableSystem;
class SpriteSystem;
class SurvivalSystem;
class TextureSystem;
class TransformSystem;
class VelocitySystem;

class GenerationSystem
{
public:
	GenerationSystem(
		ColliderSystem& colSys,
		DataSystem& dataSys,
		const GeneSystem& geneSys,
		const OverlaySystem& overlaySys,
		SelectableSystem& selSys,
		const SpriteSystem& spriteSys,
		SurvivalSystem& survSys,
		const TextureSystem& textureSys,
		const TransformSystem& tformSys
	) :
		colliderSystem(colSys),
		dataSystem(dataSys),
		geneSystem(geneSys),
		overlaySystem(overlaySys),
		selectableSystem(selSys),
		spriteSystem(spriteSys),
		survivalSystem(survSys),
		textureSystem(textureSys),
		transformSystem(tformSys)
	{}

	int& getFoodPerGeneration() { return foodPerGeneration; } //int& nonconst for ImGui slider
	int getWaveCount() const { return waveCount; }
	int getAnimalCount() const { return animalCount; }
	int getPlantCount() const { return plantCount; }

	void begin(BeingManager& beings);
	void update(BeingManager& beings);

private:

	ColliderSystem& colliderSystem;
	DataSystem& dataSystem;
	const GeneSystem& geneSystem;
	const OverlaySystem& overlaySystem;
	SelectableSystem& selectableSystem;
	const SpriteSystem& spriteSystem;
	SurvivalSystem& survivalSystem;
	const TextureSystem& textureSystem;
	const TransformSystem& transformSystem;

	int foodPerGeneration{ INITIAL_FOOD_COUNT };
	int waveCount{ 1 };
	int animalCount{ 0 };
	int plantCount{ 0 };

	float totalSpeed{ 0.0f };
	float totalStrength{ 0.0f };
	float totalDiet{ 0.0f };
	float totalStamina{ 0.0f };
	int totalActiveBeings{ 0 };

	void newWave(BeingManager& beings);
};
