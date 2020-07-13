#include "GenerationSystem.h"
#include "SurvivalSystem.h"
#include "SelectableSystem.h"
#include "BeingManager.h"
#include "GeneSystem.h"
#include "DataSystem.h"
#include "TransformSystem.h"

#include "BeingCreateInfo.h"
#include "DefaultColours.h"
#include "Math.h"

constexpr uint32_t INITIAL_ANIMAL_COUNT = 5;
constexpr uint32_t ALLOCATED_BEING_POOL_SIZE = 2000;
constexpr float DEFAULT_PLANT_FULLNESS = 300.0f;

using BeingType = GeneComponent::BeingType;
using Trait = GeneComponent::Trait;
using Data = DataSystem::Data;

void GenerationSystem::begin(BeingManager& beings)
{
	selectableSystem.clearSelectedHandle();
	waveCount = 0;
	beings.reset();
	beings.init(ALLOCATED_BEING_POOL_SIZE);
	beings.resize(INITIAL_ANIMAL_COUNT + static_cast<uint32_t>(getFoodPerGeneration()));
	survivalSystem.setWaveOver(false);

	BeingCreateInfo beingCreateInfo{};
	beingCreateInfo.dims = glm::vec2(20.0f);
	beingCreateInfo.survivalState = SurvivalState::SEARCHING;
	beingCreateInfo.isAlive = true;
	beingCreateInfo.colour = ANIMAL_COLOUR;
	beingCreateInfo.genes = GeneComponent(BeingType::ANIMAL, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
	beingCreateInfo.stamina = 0.5f;
	beingCreateInfo.fullness = 0.0f;

	for (; beingCreateInfo.beingHandle < INITIAL_ANIMAL_COUNT; beingCreateInfo.beingHandle++)
	{
		beingCreateInfo.pos = randomPosOutsideRadius(ARENA_SIZE / 2.0f);
		beings.create(beingCreateInfo);
	}

	beingCreateInfo.dims = glm::vec2(10.0f);
	beingCreateInfo.survivalState = SurvivalState::AWAITING;
	beingCreateInfo.colour = PLANT_COLOUR;
	beingCreateInfo.isAlive = false;
	beingCreateInfo.genes = GeneComponent(BeingType::PLANT, 0.0f, 0.5f, 0.5f, 0.5f, 0.5f);
	beingCreateInfo.stamina = 0.0f;
	beingCreateInfo.fullness = DEFAULT_PLANT_FULLNESS;

	for (; beingCreateInfo.beingHandle < INITIAL_ANIMAL_COUNT + getFoodPerGeneration(); beingCreateInfo.beingHandle++)
	{
		beingCreateInfo.pos = randomPosWithinRadius(ARENA_SIZE / 2.0f);
		beings.create(beingCreateInfo);
	}

	animalCount = INITIAL_ANIMAL_COUNT;
	plantCount = foodPerGeneration;
}

void GenerationSystem::process(BeingManager& beings)
{
	if (std::vector<uint32_t> toDelete = survivalSystem.compileToDelete(beings); toDelete.size())
	{
		uint32_t& selected = selectableSystem.getSelectedHandle();
		const auto deleteBeing = [&beings, &selected](const uint32_t& handle) 
		{ 
			beings.deleteBeing(handle, selected); 
		};

		std::for_each(crbegin(toDelete), crend(toDelete), deleteBeing);
	}

	if (survivalSystem.getWaveOver())
	{
		newWave(beings);
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

	BeingCreateInfo beingCreateInfo{};
	beingCreateInfo.beingHandle = 0;

	const auto replication = [this, &beingCreateInfo, &beings](const DuplicationData& data)
	{
		beingCreateInfo.pos = data.pos;
		beingCreateInfo.isAlive = data.isAlive;
		beingCreateInfo.genes = data.genes;

		if (this->geneSystem.getBeingType(data.genes) == BeingType::ANIMAL)
		{
			beingCreateInfo.dims = glm::vec2(20.0f);

			if (data.isAlive)
			{
				beingCreateInfo.fullness = 0.0f;
				beingCreateInfo.colour = ANIMAL_COLOUR;
				beingCreateInfo.survivalState = SurvivalState::SEARCHING;
				beingCreateInfo.stamina = geneSystem.getTrait(data.genes, Trait::STAMINA);
				beings.create(beingCreateInfo);

				this->totalSpeed += this->geneSystem.getTrait(data.genes, Trait::SPEED);
				this->totalStrength += this->geneSystem.getTrait(data.genes, Trait::STRENGTH);
				this->totalDiet += this->geneSystem.getTrait(data.genes, Trait::DIET);
				this->totalStamina += this->geneSystem.getTrait(data.genes, Trait::STAMINA);
				this->totalActiveBeings++;
				this->animalCount++;
			}
			else
			{
				beingCreateInfo.colour = DEAD_ANIMAL_COLOUR;
				beingCreateInfo.survivalState = SurvivalState::AWAITING;
				beingCreateInfo.fullness = data.currentFullness;
				beingCreateInfo.stamina = 0.0f;
				beings.create(beingCreateInfo);
			}
		}
		else
		{
			beingCreateInfo.dims = glm::vec2(10.0f);
			beingCreateInfo.colour = PLANT_COLOUR;
			beingCreateInfo.survivalState = SurvivalState::AWAITING;
			beingCreateInfo.isAlive = false;
			beingCreateInfo.fullness = DEFAULT_PLANT_FULLNESS;
			beingCreateInfo.stamina = 0.0f;
			beings.create(beingCreateInfo);

			this->plantCount++;
		}

		beingCreateInfo.beingHandle++;
	};

	selectableSystem.clearSelectedHandle();

	std::vector<DuplicationData> replicatedGenes;
	survivalSystem.replicate(beings, replicatedGenes);

	uint32_t replicatedBeingsSize = replicatedGenes.size();
	uint32_t totalBeingsSize = replicatedBeingsSize + getFoodPerGeneration();
	beings.resize(totalBeingsSize);

	std::for_each(cbegin(replicatedGenes), cend(replicatedGenes), replication);

	if (animalCount == 0)
	{
		beings.resize(0);
		return;
	}

	beingCreateInfo.dims = glm::vec2(10.0f);
	beingCreateInfo.survivalState = SurvivalState::AWAITING;
	beingCreateInfo.colour = PLANT_COLOUR;
	beingCreateInfo.isAlive = false;
	beingCreateInfo.genes = GeneComponent(BeingType::PLANT, 0.0f, 0.5f, 0.5f, 0.5f, 0.5f);
	beingCreateInfo.stamina = 0.0f;
	beingCreateInfo.fullness = DEFAULT_PLANT_FULLNESS;

	while (beingCreateInfo.beingHandle < totalBeingsSize)
	{
		beingCreateInfo.pos = randomPosWithinRadius(ARENA_SIZE / 2.0f);
		beings.create(beingCreateInfo);
		beingCreateInfo.beingHandle++;
		plantCount++;
	}

	// data update
	if (uint32_t n = plantCount + animalCount; n > dataSystem.getData(Data::BEING_MAXIMUM).back())
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

	waveCount++;
}
