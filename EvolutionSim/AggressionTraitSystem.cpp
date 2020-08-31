#include "AggressionTraitSystemSpecialization.h"
#include "HealthTraitSystemSpecialization.h"
#include "StrengthTraitSystemSpecialization.h"
#include "GeneSystemSpecialization.h"
#include "GenerationSystemSpecialization.h"
#include "GameData.h"

void AggressionTraitSystem::resolveAggressionTest(Handle animalHandleA, Handle animalHandleB, float combatMultiplier, GameData& gameData)
{
	GeneComponent& geneA = gameData.beingManager.pool[animalHandleA].gene;
	GeneComponent& geneB = gameData.beingManager.pool[animalHandleB].gene;
	float aggressionA = AggressionTrait::System::getTraitValue(Gene::System::getAggressionTrait(geneA));
	float aggressionB = AggressionTrait::System::getTraitValue(Gene::System::getAggressionTrait(geneB));

	int plantCount = Generation::System::getPlantCount(gameData.generation.component);
	int meatCount = Generation::System::getPlantCount(gameData.generation.component);
	if ((!Gene::System::isSharedFamilyHistory(geneA, geneB) && aggressionA + aggressionB > 1.2f) 
		|| (plantCount == 0 && meatCount == 0)) // enough mutual aggression for a fight or no available food
	{
		auto [animalStrengthBonusA, animalStrengthBonusB] = StrengthTrait::System::getCombatBonus(animalHandleA, animalHandleB, gameData);

		animalStrengthBonusA *= combatMultiplier * gameData.configurableSettings.strengthCombatMultiplier.get();
		animalStrengthBonusB *= combatMultiplier * gameData.configurableSettings.strengthCombatMultiplier.get();

		(animalStrengthBonusA > animalStrengthBonusB) ? 
			HealthTrait::System::depleteCurrentHealth(Gene::System::getHealthTrait(geneB), animalStrengthBonusA) :
			HealthTrait::System::depleteCurrentHealth(Gene::System::getHealthTrait(geneA), animalStrengthBonusB);
	}
}