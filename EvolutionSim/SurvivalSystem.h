#pragma once
#include <algorithm>
#include <vector>
#include <glm\ext\vector_float2.hpp>

#include "DefaultColours.h"
#include "SurivalComponent.h"
#include "GeneComponent.h"

struct DuplicationData;
class TransformSystem;
class PartitionSystem;
class VelocitySystem;
class GeneSystem;
class SpriteSystem;
class BeingManager;

class SurvivalSystem
{
	using Trait = GeneComponent::Trait;
	using BeingType = GeneComponent::BeingType;

public:
	SurvivalSystem(
		const TransformSystem& tformSys,
		VelocitySystem& velSys,
		GeneSystem& geneSys,
		SpriteSystem& spriteSys,
		PartitionSystem& partitionSys
	) :
		transformSystem(tformSys),
		velocitySystem(velSys),
		geneSystem(geneSys),
		spriteSystem(spriteSys),
		partitionSystem(partitionSys)
	{}

	bool getIsAlive(const SurvivalComponent& component) const { return component.isAlive; }
	bool getExists(const SurvivalComponent& component) const { return component.exists; }
	bool getWaveOver() const { return waveOver; }
	void setWaveOver(const bool set) { waveOver = set; }
	SurvivalState getSurvivalState(const SurvivalComponent& component) const { return component.state; }
	void setSurvivalState(SurvivalComponent& component, const SurvivalState state) const { component.state = state; }
	void setIsAlive(SurvivalComponent& component, const bool isAlive) const { component.isAlive = isAlive; }
	void setExists(SurvivalComponent& component, const bool exists) const { component.exists = exists; }
	float getBeingEnergy(const SurvivalComponent& component) const { return component.energy; }
	float getBeingFullness(const SurvivalComponent& component) const { return component.fullness; }
	void modifyFullness(SurvivalComponent& component, const float modify) const { component.fullness += modify; }

	void eatFood(SurvivalComponent& survivalA, SurvivalComponent& survivalB, const float maxFullness, float modify) const;

	void process(BeingManager& beings, long long dt);

	std::vector<uint32_t> compileToDelete(const BeingManager& beings);

	// If anything is not alive, it won't mutate - therefore it's food
	void replicate(const BeingManager& beings, std::vector<DuplicationData>& duplicationData) const;

	// called from collider system
	void eating(float dt, BeingManager& beings, const uint32_t animalHandle, const uint32_t foodHandle);

private:
	const TransformSystem& transformSystem;
	const VelocitySystem& velocitySystem;
	GeneSystem& geneSystem;
	SpriteSystem& spriteSystem; //for changing colour of dead entities
	PartitionSystem& partitionSystem;

	bool huntOver = false;
	bool waveOver = false;

	enum class WaveState
	{
		NOTHING_ACTIVE = 1 << 0,
		FOOD_ACTIVE = 1 << 1,
		SEARCHERS_ACTIVE = 1 << 2,
		RETURNERS_ACTIVE = 1 << 3,
	};

	glm::vec2 findNearestFoodToPoint(const glm::vec2& point, const BeingManager& beings, const std::vector<uint32_t>& foodHandles) const;

	float evaluateDietaryBonus(const BeingType foodType, float dietType) const;

	void seekFood(BeingManager& beings, const uint32_t beingHandle, const std::vector<uint32_t>& foodHandles) const;

	void seekHome(BeingManager& beings, const uint32_t beingHandle) const;

	//true when energy is depleted
	bool depleteEnergy(SurvivalComponent& component, float depletion) const;

	void wait(BeingManager& beings, const uint32_t beingHandle) const;

	void killSearchers(BeingManager& beings) const;
};
