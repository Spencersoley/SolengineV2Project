#include "BeingCreateInfo.h"
#include "DefaultColours.h"
#include "Math.h"
#include "ArenaSize.h"

#include "BeingManager.h"
#include "ColliderSystem.h"
#include "DataSystem.h"
#include "GenerationSystem.h"
#include "GeneSystem.h"
#include "OverlaySystem.h"
#include "SelectableSystem.h"
#include "SurvivalSystem.h"
#include "TextureSystem.h"
#include "TransformSystem.h"
#include "SpriteSystem.h"

constexpr int INITIAL_ANIMAL_COUNT = 5;
constexpr int ALLOCATED_BEING_POOL_SIZE = 800;
constexpr float DEFAULT_PLANT_FULLNESS = 300.0f;

constexpr float ANIMAL_DIMS = 25.0f;
constexpr float PLANT_DIMS = 18.0f;

constexpr float NO_FULLNESS = 0.0f;
constexpr bool ALIVE = true;

using Trait = GeneComponent::Trait;
using Data = DataSystem::Data;
using SurvivalState = SurvivalComponent::SurvivalState;
using Handle = unsigned int;
using Texture = TextureSystem::Component;

void GenerationSystem::begin(BeingManager& beings)
{
	selectableSystem.clearSelectedHandle();
	survivalSystem.setWaveOver(false);
	survivalSystem.setFoodHandlesNeedUpdate(true);

	waveCount = 0;
	beings.init(ALLOCATED_BEING_POOL_SIZE);
	size_t totalSize = INITIAL_ANIMAL_COUNT + static_cast<int>(getFoodPerGeneration());
	beings.setToSize(totalSize);

	for (Handle beingHandle = 0; beingHandle < INITIAL_ANIMAL_COUNT; ++beingHandle)
	{
		Being& being = beings.pool[beingHandle];
		survivalSystem.setSurvivalState(being.survival, SurvivalState::SEARCHING);
		survivalSystem.setIsAlive(being.survival, true);
		survivalSystem.setFullness(being.survival, 0.0f);
		survivalSystem.setStamina(being.survival, 0.5f);
		spriteSystem.setColour(being.sprite, ANIMAL_COLOUR);
		spriteSystem.setTextureID(being.sprite, textureSystem.getTextureID(Texture::BEING_1));
		geneSystem.setBeingType(being.gene, BeingType::ANIMAL);
		geneSystem.setTrait(being.gene, Trait::SPEED, 0.5f);
		geneSystem.setTrait(being.gene, Trait::STAMINA, 0.5f);
		geneSystem.setTrait(being.gene, Trait::STRENGTH, 0.5f);
		geneSystem.setTrait(being.gene, Trait::DIET, 0.5f);
		geneSystem.setTrait(being.gene, Trait::HUNGER, 0.5f);
		transformSystem.setPos(being.transform, randomPosOutsideRadius(ARENA_SIZE / 2.0f));
		transformSystem.setDims(being.transform, glm::vec2(ANIMAL_DIMS));
	}

	for (Handle beingHandle = INITIAL_ANIMAL_COUNT; beingHandle < totalSize; ++beingHandle)
	{
		Being& being = beings.pool[beingHandle];
		survivalSystem.setSurvivalState(being.survival, SurvivalState::AWAITING);
		survivalSystem.setIsAlive(being.survival, false);
		survivalSystem.setFullness(being.survival, DEFAULT_PLANT_FULLNESS);
		survivalSystem.setStamina(being.survival, 0.0f);
		spriteSystem.setColour(being.sprite, PLANT_COLOUR);
		spriteSystem.setTextureID(being.sprite, textureSystem.getTextureID(Texture::PLANT));
		geneSystem.setBeingType(being.gene, BeingType::PLANT);
		geneSystem.setTrait(being.gene, Trait::SPEED, 0.0f);
		geneSystem.setTrait(being.gene, Trait::STAMINA, 0.0f);
		geneSystem.setTrait(being.gene, Trait::STRENGTH, 0.0f);
		geneSystem.setTrait(being.gene, Trait::DIET, 0.0f);
		geneSystem.setTrait(being.gene, Trait::HUNGER, 0.0f);
		transformSystem.setPos(being.transform, randomPosWithinRadius(ARENA_SIZE / 2.0f));
		transformSystem.setDims(being.transform, glm::vec2(PLANT_DIMS));
	};

	animalCount = INITIAL_ANIMAL_COUNT;
	plantCount = foodPerGeneration;
}

void GenerationSystem::update(BeingManager& beings)
{
	std::set<Handle>& toDelete = colliderSystem.getToDelete(beings);
	if (survivalSystem.getWaveOver())
	{
		newWave(beings);
		toDelete.clear();
		return;
	}

	if (!toDelete.empty())
	{
		Handle& selected = selectableSystem.getSelectedHandle();
		const auto deleteBeing = [&beings, &selected](const Handle& handle)
		{
			beings.deleteBeing(handle, selected);
		};

		std::for_each(crbegin(toDelete), crend(toDelete), deleteBeing);

		survivalSystem.setFoodHandlesNeedUpdate(true);

		toDelete.clear();
	}
}

void GenerationSystem::newWave(BeingManager& beings)
{
	animalCount = 0;
	plantCount = 0;
	totalSpeed = 0.0f;
	totalStrength = 0.0f;
	totalDiet = 0.0f;
	totalStamina = 0.0f;
	totalActiveBeings = 0;

	selectableSystem.clearSelectedHandle();

	const size_t oldSize = beings.pool.size();
	for (Handle beingHandle = 0; beingHandle < oldSize; ++beingHandle)
	{
		Being& being = beings.pool[beingHandle];
		if (survivalSystem.getIsAlive(being.survival))
		{
			geneSystem.mutate(being.gene);
			survivalSystem.setSurvivalState(being.survival, SurvivalState::SEARCHING);
			survivalSystem.setFullness(being.survival, 0.0f);
			survivalSystem.setStamina(being.survival, geneSystem.getTrait(being.gene, Trait::STAMINA));
			spriteSystem.setTextureID(being.sprite, textureSystem.getTextureID(Texture::BEING_1));
			beings.pool.push_back(being);

			this->totalDiet += 2 * geneSystem.getTrait(being.gene, Trait::DIET);
			this->totalSpeed += 2 * geneSystem.getTrait(being.gene, Trait::SPEED);
			this->totalStamina += 2 * geneSystem.getTrait(being.gene, Trait::STAMINA);
			this->totalStrength += 2 * geneSystem.getTrait(being.gene, Trait::STRENGTH);
			this->totalActiveBeings += 2;
			animalCount += 2;
		}
		else if (geneSystem.getBeingType(being.gene) == BeingType::ANIMAL)
		{
			//animal
			//dead animal count++
		}
		else
		{
			//plant
			survivalSystem.setFullness(being.survival, DEFAULT_PLANT_FULLNESS);
			plantCount++;
		}
	}

	if (animalCount == 0)
	{
		beings.resize(0);
		return;
	}

	size_t animalSize = beings.pool.size();
	size_t totalBeingsSize = animalSize + getFoodPerGeneration();
	beings.resize(totalBeingsSize);
	plantCount += getFoodPerGeneration();
	unsigned int target = getFoodPerGeneration();
	for (unsigned int i = 0; i < target; ++i)
	{
		Being& being = beings.pool[animalSize + i];
		survivalSystem.setSurvivalState(being.survival, SurvivalState::AWAITING);
		survivalSystem.setIsAlive(being.survival, false);
		survivalSystem.setFullness(being.survival, DEFAULT_PLANT_FULLNESS);
		survivalSystem.setStamina(being.survival, 0.0f);
		spriteSystem.setColour(being.sprite, PLANT_COLOUR);
		spriteSystem.setTextureID(being.sprite, textureSystem.getTextureID(Texture::PLANT));
		geneSystem.setBeingType(being.gene, BeingType::PLANT);
		geneSystem.setTrait(being.gene, Trait::SPEED, 0.0f);
		geneSystem.setTrait(being.gene, Trait::STAMINA, 0.0f);
		geneSystem.setTrait(being.gene, Trait::STRENGTH, 0.0f);
		geneSystem.setTrait(being.gene, Trait::DIET, 0.0f);
		geneSystem.setTrait(being.gene, Trait::HUNGER, 0.0f);
		transformSystem.setPos(being.transform, randomPosWithinRadius(ARENA_SIZE / 2.0f));
		transformSystem.setDims(being.transform, glm::vec2(PLANT_DIMS));
	}

	// data update
	if (unsigned int n = plantCount + animalCount; n > dataSystem.getData(Data::BEING_MAXIMUM).back())
	{
		dataSystem.clear(Data::BEING_MAXIMUM);
		dataSystem.addPoint(Data::BEING_MAXIMUM, static_cast<float>(n));
	}

	dataSystem.addPoint(Data::SPEED, totalSpeed / totalActiveBeings);
	dataSystem.addPoint(Data::DIET, totalDiet / totalActiveBeings);
	dataSystem.addPoint(Data::STRENGTH, totalStrength / totalActiveBeings);
	dataSystem.addPoint(Data::STAMINA, totalStamina / totalActiveBeings);
	dataSystem.addPoint(Data::ANIMAL_POPULATION, static_cast<float>(totalActiveBeings));
	dataSystem.addPoint(Data::PLANT_POPULATION, static_cast<float>(plantCount));

	overlaySystem.updateOverlay(beings);
	survivalSystem.setFoodHandlesNeedUpdate(true);

	waveCount++;
}
