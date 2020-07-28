#pragma once
#include <vector>
#include <glm\ext\vector_float2.hpp>

#include "DefaultColours.h"
#include "SurvivalComponent.h"
#include "GeneComponent.h"
#include <set>

struct DuplicationData;
class BeingManager;
class TextureSystem;
class TransformSystem;
class PartitionSystem;
class VelocitySystem;
class GeneSystem;
class SpriteSystem;

class SurvivalSystem
{
	using Trait = GeneComponent::Trait;
	using BeingType = GeneComponent::BeingType;
	using SurvivalState = SurvivalComponent::SurvivalState;
	using Handle = unsigned int;

public:
	SurvivalSystem(
		const TextureSystem& textureSys,
		const TransformSystem& tformSys,
		VelocitySystem& velSys,
		GeneSystem& geneSys,
		SpriteSystem& spriteSys
	) :
		textureSystem(textureSys),
		transformSystem(tformSys),
		velocitySystem(velSys),
		geneSystem(geneSys),
		spriteSystem(spriteSys)
	{
	}

	bool getIsAlive(const SurvivalComponent& component) const;
	bool getWaveOver() const { return waveOver; }
	void setWaveOver(const bool set) { waveOver = set; }
	SurvivalState getSurvivalState(const SurvivalComponent& component) const;
	void setSurvivalState(SurvivalComponent& component, const SurvivalState state) const;
	void setIsAlive(SurvivalComponent& component, const bool isAlive) const;
	void setFullness(SurvivalComponent& component, float set) const;
	float getBeingEnergy(const SurvivalComponent& component) const;
	float getBeingFullness(const SurvivalComponent& component) const;
	void setStamina(SurvivalComponent& component, float set) const;

	void setFoodHandlesNeedUpdate(bool set) { foodHandlesNeedUpdate = set; }
	bool getFoodHandlesNeedUpdate() const { return foodHandlesNeedUpdate; }

	void update(BeingManager& beings, uint32_t dt);

private:
	const TextureSystem& textureSystem;
	const TransformSystem& transformSystem;
	const VelocitySystem& velocitySystem;
	GeneSystem& geneSystem;
	SpriteSystem& spriteSystem; //for changing colour of dead entities

	bool huntOver = false;
	bool waveOver = false;

	enum class WaveState
	{
		NOTHING_ACTIVE = 1 << 0,
		FOOD_ACTIVE = 1 << 1,
		SEARCHERS_ACTIVE = 1 << 2,
		RETURNERS_ACTIVE = 1 << 3,
	};

	std::vector<Handle> foodHandles{};
	bool foodHandlesNeedUpdate{ true };

	std::set<Handle> toDelete{};

	glm::vec2 findNearestFoodToPoint(const glm::vec2& point, const BeingManager& beings, const std::vector<Handle>& foodHandles) const;

	bool depleteEnergy(SurvivalComponent& component, float depletion) const;

	void wait(Being& being) const;

	void killAnimal(Being& being);
};
