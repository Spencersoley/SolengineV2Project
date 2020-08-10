#include "Math.h"
#include "SelectedTracker.h"
#include "BeingManager.h"
#include "GenerationManager.h"
#include "DataPointManager.h"
#include "OverlayConfig.h"
#include "GeneEnum.h"
#include "TextureLibrary.h"
#include "DefaultColours.h"

#include "SurvivalSystemImplementation.h"
#include "DataSystemImplementation.h"
#include "GenerationSystemImplementation.h"
#include "GeneSystemImplementation.h"
#include "OverlaySystemImplementation.h"
#include "SelectableSystemImplementation.h"
#include "TransformSystemImplementation.h"
#include "SpriteSystemImplementation.h"
#include "TextureLoaderSystemImplementation.h"

constexpr int INITIAL_ANIMAL_COUNT = 5;
constexpr int ALLOCATED_BEING_POOL_SIZE = 800;
constexpr float DEFAULT_PLANT_FULLNESS = 300.0f;

constexpr float ANIMAL_DIMS = 25.0f;
constexpr float PLANT_DIMS = 18.0f;

constexpr float NO_FULLNESS = 0.0f;
constexpr bool ALIVE = true;

using Trait = Gene::Trait;
using SurvivalState = SurvivalComponent::SurvivalState;
using Handle = unsigned int;
using BeingType = Gene::BeingType;

void GenerationSystem::begin(BeingManager& beings, GenerationManager& generationManager, SelectedTracker& selected, TextureLibrary& textureLibrary)
{
	Selectable::System::clearSelectedHandle(selected.component);
	Generation::System::setWaveOver(generationManager.component, false);
	Generation::System::setFoodHandlesNeedUpdate(generationManager.component, true);

	Generation::System::setWaveCount(generationManager.component, 0);
	beings.init(ALLOCATED_BEING_POOL_SIZE);
	int initialFoodCount = Generation::System::getFoodPerGeneration(generationManager.component);
	size_t totalSize = INITIAL_ANIMAL_COUNT + initialFoodCount;
	beings.setToSize(totalSize);

	for (Handle beingHandle = 0; beingHandle < INITIAL_ANIMAL_COUNT; ++beingHandle)
	{
		Being& being = beings.pool[beingHandle];
		Survival::System::setSurvivalState(being.survival, SurvivalState::SEARCHING);
		Survival::System::setIsAlive(being.survival, true);
		Survival::System::setFullness(being.survival, 0.0f);
		Survival::System::setStamina(being.survival, 0.5f);
		Sprite::System::setColour(being.sprite, ANIMAL_COLOUR);
		Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::BEING_1, textureLibrary));
		Gene::System::setBeingType(being.gene, BeingType::ANIMAL);
		Gene::System::setTrait(being.gene, Trait::SPEED, 0.5f);
		Gene::System::setTrait(being.gene, Trait::STAMINA, 0.5f);
		Gene::System::setTrait(being.gene, Trait::STRENGTH, 0.5f);
		Gene::System::setTrait(being.gene, Trait::DIET, 0.5f);
		Gene::System::setTrait(being.gene, Trait::HUNGER, 0.5f);
		Transform::System::setPos(being.transform, randomPosOutsideRadius(ARENA_SIZE / 2.0f));
		Transform::System::setDims(being.transform, glm::vec2(ANIMAL_DIMS));
	}

	for (Handle beingHandle = INITIAL_ANIMAL_COUNT; beingHandle < totalSize; ++beingHandle)
	{
		Being& being = beings.pool[beingHandle];
		Survival::System::setSurvivalState(being.survival, SurvivalState::AWAITING);
		Survival::System::setIsAlive(being.survival, false);
		Survival::System::setFullness(being.survival, DEFAULT_PLANT_FULLNESS);
		Survival::System::setStamina(being.survival, 0.0f);
		Sprite::System::setColour(being.sprite, PLANT_COLOUR);
		Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::PLANT, textureLibrary));
		Gene::System::setBeingType(being.gene, BeingType::PLANT);
		Gene::System::setTrait(being.gene, Trait::SPEED, 0.0f);
		Gene::System::setTrait(being.gene, Trait::STAMINA, 0.0f);
		Gene::System::setTrait(being.gene, Trait::STRENGTH, 0.0f);
		Gene::System::setTrait(being.gene, Trait::DIET, 0.0f);
		Gene::System::setTrait(being.gene, Trait::HUNGER, 0.0f);
		Transform::System::setPos(being.transform, randomPosWithinRadius(ARENA_SIZE / 2.0f));
		Transform::System::setDims(being.transform, glm::vec2(PLANT_DIMS));
	};

	Generation::System::setAnimalCount(generationManager.component, INITIAL_ANIMAL_COUNT);
	Generation::System::setPlantCount(generationManager.component, initialFoodCount);
}

void GenerationSystem::update(BeingManager& beings, DataPointManager& data, GenerationManager& generationManager, OverlayConfig& overlayConfig, SelectedTracker& selected, TextureLibrary& textureLibrary, std::set<Handle>& handlesToDelete)
{
	if (Generation::System::getWaveOver(generationManager.component))
	{
		newWave(beings, data, generationManager, overlayConfig, selected, textureLibrary);
		handlesToDelete.clear();
		return;
	}

	if (!handlesToDelete.empty())
	{
		Handle selectedHandle = Selectable::System::getHandle(selected.component);
		const auto deleteBeing = [&beings, &selectedHandle](const Handle& handle)
		{
			const size_t size = beings.getSize();
			if (handle >= size)
			{
				throw std::exception("handle outside of being pool range");
			}
			if (selectedHandle == size - 1) selectedHandle = handle;
			if (selectedHandle == handle) selectedHandle = size;

			beings.deleteBeing(handle, selectedHandle);
		};

		std::for_each(crbegin(handlesToDelete), crend(handlesToDelete), deleteBeing);

		Selectable::System::setHandle(selected.component, selectedHandle);

		Generation::System::setFoodHandlesNeedUpdate(generationManager.component, true);

		handlesToDelete.clear();
	}
}

void GenerationSystem::newWave(BeingManager& beings, DataPointManager& data, GenerationManager& generation, OverlayConfig& overlayConfig, SelectedTracker& selected, TextureLibrary& textureLibrary)
{
	float totalDiet{ 0.0f }, totalSpeed{ 0.0f }, totalStamina{ 0.0f }, totalStrength{ 0.0f };
	int totalActiveBeings{ 0 }, animalCount{ 0 }, plantCount{ 0 };

	Selectable::System::clearSelectedHandle(selected.component);

	const size_t oldSize = beings.pool.size();
	for (Handle beingHandle = 0; beingHandle < oldSize; ++beingHandle)
	{
		Being& being = beings.pool[beingHandle];
		if (Survival::System::getIsAlive(being.survival))
		{
			Gene::System::mutate(being.gene);
			Survival::System::setSurvivalState(being.survival, SurvivalState::SEARCHING);
			Survival::System::setFullness(being.survival, 0.0f);
			Survival::System::setStamina(being.survival, Gene::System::getTrait(being.gene, Trait::STAMINA));
			Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::BEING_1, textureLibrary));
			beings.pool.push_back(being);

			totalDiet += 2 * Gene::System::getTrait(being.gene, Trait::DIET);
			totalSpeed += 2 * Gene::System::getTrait(being.gene, Trait::SPEED);
			totalStamina += 2 * Gene::System::getTrait(being.gene, Trait::STAMINA);
			totalStrength += 2 * Gene::System::getTrait(being.gene, Trait::STRENGTH);
			totalActiveBeings += 2;
			animalCount += 2;
		}
		else if (Gene::System::getBeingType(being.gene) == BeingType::ANIMAL)
		{
			//animal
			//dead animal count++
		}
		else
		{
			//plant
			Survival::System::setFullness(being.survival, DEFAULT_PLANT_FULLNESS);
			plantCount++;
		}
	}

	if (animalCount == 0)
	{
		beings.resize(0);
		return;
	}

	size_t animalSize = beings.pool.size();
	size_t foodPerGeneration = Generation::System::getFoodPerGeneration(generation.component);
	size_t totalBeingsSize = animalSize + foodPerGeneration;
	beings.resize(totalBeingsSize);
	plantCount += foodPerGeneration;
	unsigned int target = foodPerGeneration;
	for (unsigned int i = 0; i < target; ++i)
	{
		Being& being = beings.pool[animalSize + i];
		Survival::System::setSurvivalState(being.survival, SurvivalState::AWAITING);
		Survival::System::setIsAlive(being.survival, false);
		Survival::System::setFullness(being.survival, DEFAULT_PLANT_FULLNESS);
		Survival::System::setStamina(being.survival, 0.0f);
		Sprite::System::setColour(being.sprite, PLANT_COLOUR);
		Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::PLANT, textureLibrary));
		Gene::System::setBeingType(being.gene, BeingType::PLANT);
		Gene::System::setTrait(being.gene, Trait::SPEED, 0.0f);
		Gene::System::setTrait(being.gene, Trait::STAMINA, 0.0f);
		Gene::System::setTrait(being.gene, Trait::STRENGTH, 0.0f);
		Gene::System::setTrait(being.gene, Trait::DIET, 0.0f);
		Gene::System::setTrait(being.gene, Trait::HUNGER, 0.0f);
		Transform::System::setPos(being.transform, randomPosWithinRadius(ARENA_SIZE / 2.0f));
		Transform::System::setDims(being.transform, glm::vec2(PLANT_DIMS));
	}

	// data update
	if (unsigned int n = plantCount + animalCount; n > Data::System::getData(data.beingMaximum).back())
	{
		Data::System::clear(data.beingMaximum);
		Data::System::addPoint(data.beingMaximum, static_cast<float>(n));
	}

	Data::System::addPoint(data.speed, totalSpeed / totalActiveBeings);
	Data::System::addPoint(data.diet, totalDiet / totalActiveBeings);
	Data::System::addPoint(data.strength, totalStrength / totalActiveBeings);
	Data::System::addPoint(data.stamina, totalStamina / totalActiveBeings);
	Data::System::addPoint(data.animalPopulation, static_cast<float>(totalActiveBeings));
	Data::System::addPoint(data.plantPopulation, static_cast<float>(plantCount));

	Overlay::System::updateOverlay(beings, overlayConfig);
	Generation::System::setFoodHandlesNeedUpdate(generation.component, true);

	Generation::System::setWaveCount(generation.component, Generation::System::getWaveCount(generation.component) + 1);
}
