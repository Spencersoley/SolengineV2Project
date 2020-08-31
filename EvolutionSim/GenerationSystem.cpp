#include <RandomNumberGenerator.h>
#include "Math.h"
#include "GameData.h"
#include "DefaultColours.h"
#include "SurvivalSystemSpecialization.h"
#include "DataSystemSpecialization.h"
#include "GenerationSystemSpecialization.h"
#include "GeneSystemSpecialization.h"
#include "OverlaySystemSpecialization.h"
#include "SelectableSystemSpecialization.h"
#include "TransformSystemSpecialization.h"
#include "SpriteSystemSpecialization.h"
#include "TextureLoaderSystemSpecialization.h"
#include "FoodSystemSpecialization.h"
#include "AggressionTraitSystemSpecialization.h"
#include "DietTraitSystemSpecialization.h"
#include "HealthTraitSystemSpecialization.h"
#include "HungerTraitSystemSpecialization.h"
#include "IntelligenceTraitSystemSpecialization.h"
#include "SpeedTraitSystemSpecialization.h"
#include "StaminaTraitSystemSpecialization.h"
#include "StrengthTraitSystemSpecialization.h"

constexpr int INITIAL_ANIMAL_COUNT = 1;
constexpr int ALLOCATED_BEING_POOL_SIZE = 800;


constexpr float ANIMAL_DIMS = 25.0f;
constexpr float PLANT_DIMS = 18.0f;

constexpr int TOXIC_PLANT_INITIAL = 1;
constexpr int TOXIC_PLANT_INTERVAL = 12;

using SurvivalState = SurvivalComponent::SurvivalState;
using Handle = unsigned int;

void GenerationSystem::begin(GameData& gameData, SolengineV2::RandomNumberGenerator& rng) const
{
	Selectable::System::clearSelectedHandle(gameData.selectedTracker.component);
	GenerationComponent& generationComponent = gameData.generation.component;
	Generation::System::setWaveOver(generationComponent, false);

	Generation::System::setWaveCount(generationComponent, 0);
	gameData.beingManager.init(ALLOCATED_BEING_POOL_SIZE);
	gameData.foodManager.init(ALLOCATED_BEING_POOL_SIZE);

	float arenaRadius = gameData.configurableSettings.arenaDiameter.get() / 2.0f;
	Manager<BeingEntity>& beingManager = gameData.beingManager;
	gameData.beingManager.setToSize(INITIAL_ANIMAL_COUNT);
	for (Handle beingHandle = 0; beingHandle < INITIAL_ANIMAL_COUNT; ++beingHandle)
	{
		BeingEntity& being = beingManager.pool[beingHandle];
		Survival::System::setSurvivalState(being.survival, SurvivalState::SEARCHING);
		Survival::System::setIsPoisoned(being.survival, false);
		Gene::System::setAllTraits(being.gene, 0.5f);

		HealthTrait::System::setCurrentHealthToMax(Gene::System::getHealthTrait(being.gene));
		HungerTrait::System::setFullness(Gene::System::getHungerTrait(being.gene), 0.0f);
		StaminaTrait::System::setEnergyToFull(Gene::System::getStaminaTrait(being.gene));
		Sprite::System::setColour(being.sprite, COLOUR::ANIMAL);
		Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::BEING_1, gameData));
		Transform::System::setPos(being.transform, Math::randomPosOutsideRadius(arenaRadius, rng.getRandomZeroToOne(), rng.getRandomZeroToOne()));
		Transform::System::setDims(being.transform, glm::vec2(ANIMAL_DIMS));
	}

	Manager<FoodEntity>& foodManager = gameData.foodManager;
	size_t initialFoodCount = gameData.configurableSettings.foodPerGeneration.get();
	gameData.foodManager.setToSize(initialFoodCount);

	float plantSaturation = gameData.configurableSettings.plantSaturation.get();
	for (Handle foodHandle = 0; foodHandle < initialFoodCount; ++foodHandle)
	{
		FoodEntity& food = foodManager.pool[foodHandle];
		Sprite::System::setColour(food.sprite, COLOUR::PLANT);
		Sprite::System::setTextureID(food.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::PLANT, gameData));
		Food::System::setSaturation(food.food, plantSaturation);
		Transform::System::setPos(food.transform, Math::randomPosWithinRadius(arenaRadius, rng.getRandomZeroToOne(), rng.getRandomZeroToOne()));
		Transform::System::setDims(food.transform, glm::vec2(PLANT_DIMS));
	};

	Generation::System::setAnimalCount(generationComponent, INITIAL_ANIMAL_COUNT);
	Generation::System::setPlantCount(generationComponent, initialFoodCount);
}

void GenerationSystem::update(GameData& gameData, SolengineV2::RandomNumberGenerator& rng, const std::chrono::microseconds& deltaTime) const
{
	GenerationComponent& generationComponent = gameData.generation.component;
	if (Generation::System::getWaveOver(generationComponent))
	{
		newWave(gameData, rng);
	}

	SelectedComponent& selectedComponent = gameData.selectedTracker.component;
	Handle selectedFood = Selectable::System::getFoodHandle(selectedComponent);
	int plantCount{ 0 }, meatCount{ 0 }, toxicCount{ 0 };
	std::vector<FoodEntity>& foodPool = gameData.foodManager.pool;
	size_t foodSize = foodPool.size();
	for (int i = foodSize - 1; i >= 0; --i)
	{
		FoodComponent& food = foodPool[i].food;
		if (Food::System::getSaturation(food) <= 0)
		{
			if (i == selectedFood)
			{
				Selectable::System::setFoodHandle(selectedComponent, UINT_MAX);
			}
			foodPool[i] = foodPool.back();
			foodPool.pop_back();
		}
		else if (Food::System::getIsToxic(food))
		{
			toxicCount++;
		}
		else if (Food::System::getFoodType(food) == FoodType::MEAT)
		{
			meatCount++;
		}
		else if (Food::System::getFoodType(food) == FoodType::PLANT)
		{
			plantCount++;
		}
	}

	Generation::System::setPlantCount(gameData.generation.component, plantCount);
	Generation::System::setMeatCount(gameData.generation.component, meatCount);
	Generation::System::setToxicCount(gameData.generation.component, toxicCount);

	std::vector<BeingEntity>& animalPool = gameData.beingManager.pool;
	size_t animalSize = animalPool.size();
	float poisonDamage = gameData.configurableSettings.poisonDamageRateMultiplier.get() * deltaTime.count();
	for (int beingHandle = animalSize - 1; beingHandle >= 0; --beingHandle)
	{
		if (BeingEntity& being = animalPool[beingHandle]; !HealthTrait::System::getHasHealth(Gene::System::getHealthTrait(being.gene)))
		{
			killAnimal(being, static_cast<Handle>(beingHandle), gameData);
			if (Survival::System::getIsPoisoned(being.survival))
			{
				//poison death
				Data::System::modifyCurrentPoint(gameData.dataPoint.causeOfDeathPoison, 1);
				Food::System::setIsToxic(foodPool.back().food, true);
				Sprite::System::setColour(foodPool.back().sprite, COLOUR::TOXIC);
			}
			else
			{
				//combat death
				Data::System::modifyCurrentPoint(gameData.dataPoint.causeOfDeathPredator, 1);
				Sprite::System::setColour(foodPool.back().sprite, COLOUR::MEAT);
			}
		}
		else if (StaminaTrait::System::getEnergy(Gene::System::getStaminaTrait(being.gene)) <= 0.0f)
		{
			// starvation death
			killAnimal(being, static_cast<Handle>(beingHandle), gameData);
			Data::System::modifyCurrentPoint(gameData.dataPoint.causeOfDeathStarvation, 1);
			Sprite::System::setColour(foodPool.back().sprite, COLOUR::MEAT);
		}
		else if (Survival::System::getIsPoisoned(being.survival))
		{
			HealthTrait::System::depleteCurrentHealth(Gene::System::getHealthTrait(being.gene), poisonDamage);
		}
	}
}

void GenerationSystem::newWave(GameData& gameData, SolengineV2::RandomNumberGenerator& rng) const
{
	float totalAggression{ 0.0f }, totalDiet{ 0.0f }, totalHealth{ 0.0f }, totalHunger{ 0.0f }, totalIntelligence{ 0.0f }, totalSpeed{ 0.0f }, totalStamina{ 0.0f }, totalStrength{ 0.0f };
	int totalActiveBeings{ 0 }, animalCount{ 0 }, plantCount{ 0 };

	/// BEINGS
	Manager<BeingEntity>& beingManager = gameData.beingManager;
	const size_t oldSize = beingManager.pool.size();
	for (Handle beingHandle = 0; beingHandle < oldSize; ++beingHandle)
	{
		BeingEntity& being = beingManager.pool[beingHandle];
		if (HealthTrait::System::getCurrentHealth(Gene::System::getHealthTrait(being.gene)) > 0)
		{
			Gene::System::mutate(being.gene, rng);
			Survival::System::setSurvivalState(being.survival, SurvivalState::SEARCHING);
			HealthTrait::System::setCurrentHealthToMax(Gene::System::getHealthTrait(being.gene));
			HungerTrait::System::setFullness(Gene::System::getHungerTrait(being.gene), 0.0f);
			StaminaTrait::System::setEnergyToFull(Gene::System::getStaminaTrait(being.gene));
			Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::BEING_1, gameData));
			beingManager.pool.push_back(being);

			totalAggression += 2 * AggressionTrait::System::getTraitValue(Gene::System::getAggressionTrait(being.gene));
			totalDiet += 2 * DietTrait::System::getTraitValue(Gene::System::getDietTrait(being.gene));
			totalHealth += 2 * HealthTrait::System::getTraitValue(Gene::System::getHealthTrait(being.gene));
			totalHunger += 2 * HungerTrait::System::getTraitValue(Gene::System::getHungerTrait(being.gene));
			totalIntelligence += 2 * IntelligenceTrait::System::getTraitValue(Gene::System::getIntelligenceTrait(being.gene));
			totalSpeed += 2 * SpeedTrait::System::getTraitValue(Gene::System::getSpeedTrait(being.gene));
			totalStamina += 2 * StaminaTrait::System::getTraitValue(Gene::System::getStaminaTrait(being.gene));
			totalStrength += 2 * StrengthTrait::System::getTraitValue(Gene::System::getStrengthTrait(being.gene));
			totalActiveBeings += 2;
			animalCount += 2;
		}
	}

	/// CURRENT FOOD
	Manager<FoodEntity>& foodManager = gameData.foodManager;
	float toxicDeterioration{ gameData.configurableSettings.toxicDeteriorationRateMultiplier.get() };
	float plantSaturation = gameData.configurableSettings.plantSaturation.get();
	const size_t foodSize = foodManager.pool.size();
	for (Handle foodHandle = 0; foodHandle < foodSize; ++foodHandle)
	{
		FoodEntity& food = foodManager.pool[foodHandle];
		if (Food::System::getIsToxic(food.food))
		{
			Food::System::depleteSaturation(food.food, toxicDeterioration);
		}
		else
		{
			Food::System::setSaturation(food.food, plantSaturation);
		}
	}

	if (animalCount == 0)
	{
		beingManager.resize(0);
		return;
	}

	/// ADDITIONAL FOOD
	GenerationComponent& generationComponent = gameData.generation.component;
	size_t foodPerGeneration = gameData.configurableSettings.foodPerGeneration.get();
	size_t totalFoodSize = foodSize + foodPerGeneration;
	foodManager.resize(totalFoodSize);
	plantCount += foodPerGeneration;
	float arenaRadius = gameData.configurableSettings.arenaDiameter.get() / 2.0f;
	for (Handle foodHandle = foodSize; foodHandle < totalFoodSize; ++foodHandle)
	{
		FoodEntity& food = foodManager.pool[foodHandle];
		Food::System::setSaturation(food.food, plantSaturation);
		if (foodHandle == TOXIC_PLANT_INITIAL || foodHandle % TOXIC_PLANT_INTERVAL == 0)
		{
			Sprite::System::setColour(food.sprite, COLOUR::TOXIC);
			Food::System::setIsToxic(food.food, true);
		}
		else
		{
			Sprite::System::setColour(food.sprite, COLOUR::PLANT);
		}
		Sprite::System::setTextureID(food.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::PLANT, gameData));
		Transform::System::setPos(food.transform, Math::randomPosWithinRadius(arenaRadius, rng.getRandomZeroToOne(), rng.getRandomZeroToOne()));
		Transform::System::setDims(food.transform, glm::vec2(PLANT_DIMS));
	}

	Overlay::System::updateOverlay(gameData);

	Generation::System::setWaveCount(generationComponent, Generation::System::getWaveCount(generationComponent) + 1);

	Generation::System::setAnimalCount(generationComponent, animalCount);
	Generation::System::setPlantCount(generationComponent, plantCount);

	/// DATA
	Data::System::modifyCurrentPoint(gameData.dataPoint.aggression, totalAggression / totalActiveBeings);
	Data::System::modifyCurrentPoint(gameData.dataPoint.diet, totalDiet / totalActiveBeings);
	Data::System::modifyCurrentPoint(gameData.dataPoint.health, totalHealth / totalActiveBeings);
	Data::System::modifyCurrentPoint(gameData.dataPoint.hunger, totalHunger / totalActiveBeings);
	Data::System::modifyCurrentPoint(gameData.dataPoint.intelligence, totalIntelligence / totalActiveBeings);
	Data::System::modifyCurrentPoint(gameData.dataPoint.speed, totalSpeed / totalActiveBeings);
	Data::System::modifyCurrentPoint(gameData.dataPoint.stamina, totalStamina / totalActiveBeings);
	Data::System::modifyCurrentPoint(gameData.dataPoint.strength, totalStrength / totalActiveBeings);
	Data::System::modifyCurrentPoint(gameData.dataPoint.animalPopulation, static_cast<float>(totalActiveBeings));
	Data::System::modifyCurrentPoint(gameData.dataPoint.plantPopulation, static_cast<float>(plantCount));
	int waveCount = Generation::System::getWaveCount(gameData.generation.component);
	Data::System::modifyCurrentPoint(gameData.dataPoint.waveCount, static_cast<float>(waveCount));

	float currentMax = Data::System::getCurrentPoint(gameData.dataPoint.deathMaximum);
	float potentialNewMax = std::max(Data::System::getCurrentPoint(gameData.dataPoint.causeOfDeathPredator),
		std::max(Data::System::getCurrentPoint(gameData.dataPoint.causeOfDeathPoison),
			Data::System::getCurrentPoint(gameData.dataPoint.causeOfDeathStarvation)));
	if (potentialNewMax > currentMax)
	{
		Data::System::setCurrentPoint(gameData.dataPoint.deathMaximum, potentialNewMax * 1.1f);
	}

	Data::System::accumulateAllData(gameData.dataPoint);

	DataPointEntity& dataPoint = gameData.dataPoint;
	if (unsigned int n = plantCount + animalCount; n > Data::System::getCurrentPoint(dataPoint.beingMaximum))
	{
		Data::System::setCurrentPoint(gameData.dataPoint.beingMaximum, static_cast<float>(n));
	}
}

void GenerationSystem::killAnimal(BeingEntity& being, Handle beingHandle, GameData& gameData) const
{
	SelectedComponent& selectedComponent = gameData.selectedTracker.component;
	Handle selectedAnimal = Selectable::System::getSelectedAnimalHandle(selectedComponent);
	if (beingHandle == selectedAnimal)
	{
		Selectable::System::clearSelectedHandle(selectedComponent);
	}

	//createFood
	std::vector<FoodEntity>& foodPool = gameData.foodManager.pool;
	foodPool.push_back(FoodEntity());
	FoodEntity& food = foodPool.back();

	Sprite::System::setTextureID(food.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::MEAT, gameData));
	Food::System::setSaturation(food.food,
		HungerTrait::System::getFullness(Gene::System::getHungerTrait(being.gene)) +
		StaminaTrait::System::getEnergy(Gene::System::getStaminaTrait(being.gene)));
	Food::System::setFoodType(food.food, FoodType::MEAT);

	auto& pos = Transform::System::getPos(being.transform); // boundary for food spawning
	if (float distanceOutOfBounds = glm::distance(pos, { 0, 0 }) - ((gameData.configurableSettings.arenaDiameter.get() * 0.95) / 2.0f); distanceOutOfBounds > 0)
	{
		Transform::System::setPos(foodPool.back().transform, pos - (glm::normalize(pos) * distanceOutOfBounds));
	}
	else
	{
		Transform::System::setPos(foodPool.back().transform, pos);
	}

	Transform::System::setDims(foodPool.back().transform, Transform::System::getDims(being.transform));

	being = gameData.beingManager.pool.back();
	gameData.beingManager.pool.pop_back();
}