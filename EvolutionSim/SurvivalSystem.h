#pragma once
#include <vector>
#include <glm\ext\vector_float2.hpp>
#include <set>

#include "GeneEnum.h"

class BeingManager;
class GenerationManager;
class Being;
class TextureLibrary;

class SurvivalSystem
{
	using Trait = Gene::Trait;
	using BeingType = Gene::BeingType;
	using SurvivalState = SurvivalComponent::SurvivalState;
	using Handle = unsigned int;

public:
	void update(BeingManager& beings, uint32_t dt, GenerationManager& generationManager, TextureLibrary& textureLibrary);

private:

	enum class WaveState
	{
		NOTHING_ACTIVE = 1 << 0,
		FOOD_ACTIVE = 1 << 1,
		SEARCHERS_ACTIVE = 1 << 2,
		RETURNERS_ACTIVE = 1 << 3,
	};

	glm::vec2 findNearestFoodToPoint(const glm::vec2& point, const BeingManager& beings, const std::vector<Handle>& foodHandles) const;

	bool depleteEnergyAndReturnIsZero(SurvivalComponent& component, float depletion) const;

	void wait(Being& being) const;

	void killAnimal(Being& being, TextureLibrary& textureLibrary);
};
