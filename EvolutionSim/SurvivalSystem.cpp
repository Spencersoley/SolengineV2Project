#include <glm\geometric.hpp>
#include "GameData.h"
#include "DefaultColours.h"
#include "TransformSystemSpecialization.h"
#include "GeneSystemSpecialization.h"
#include "GenerationSystemSpecialization.h"
#include "SpeedTraitSystemSpecialization.h"
#include "StaminaTraitSystemSpecialization.h"
#include "HungerTraitSystemSpecialization.h"
#include "SurvivalSystemSpecialization.h"

using SurvivalState = SurvivalComponent::SurvivalState;

void SurvivalSystem::update(const std::chrono::microseconds& deltaTime, GameData& gameData)
{
	bool waveActive{ false };
	GenerationComponent& generationComponent = gameData.generation.component;
	Generation::System::setWaveOver(generationComponent, false);

	float depletion = gameData.configurableSettings.staminaDepletionRate.get() * deltaTime.count() * gameData.configurableSettings.physicsSpeed.get();
	auto arenaRadius = gameData.configurableSettings.arenaDiameter.get() / 2.0f;
	const auto processSurvivalBehaviour = [this, &waveActive, arenaRadius, depletion, &gameData](BeingEntity& being)
	{
		if (Survival::System::getSurvivalState(being.survival) == SurvivalState::AWAITING)
		{
			SpeedTrait::System::setIsMoving(Gene::System::getSpeedTrait(being.gene), false);
		}
		else 
		{
			waveActive = true;

			if (Survival::System::getSurvivalState(being.survival) == SurvivalState::SEARCHING)
			{
				// SEEK FOOD
				HungerTrait::System::seekNearestFood(being, gameData);
				StaminaTrait::System::depleteEnergy(being.gene, depletion);
			}
			else if (Survival::System::getSurvivalState(being.survival) == SurvivalState::RETURNING)
			{
				// SEEK HOME 
				const glm::vec2& pos = Transform::System::getPos(being.transform);
				if (glm::length(pos) > arenaRadius)
				{
					Survival::System::setSurvivalState(being.survival, SurvivalState::AWAITING);
					SpeedTrait::System::setIsMoving(Gene::System::getSpeedTrait(being.gene), false);
				}
				else
				{
					SpeedTrait::System::setIsMoving(Gene::System::getSpeedTrait(being.gene), true);
					SpeedTrait::System::setDirection(Gene::System::getSpeedTrait(being.gene), glm::normalize(pos));
				}
			}
		}
	};

	Manager<BeingEntity>& beingManager = gameData.beingManager;
	for_each(begin(beingManager.pool), end(beingManager.pool), processSurvivalBehaviour);

	if (!waveActive) // if no searchers, no returners
	{
		Generation::System::setWaveOver(generationComponent, true);
	}
}