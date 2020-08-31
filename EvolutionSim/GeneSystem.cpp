#include <Vertex.h>
#include <algorithm>
#include <RandomNumberGenerator.h>
#include "GeneSystemSpecialization.h"
#include "AggressionTraitSystemSpecialization.h"
#include "DietTraitSystemSpecialization.h"
#include "HealthTraitSystemSpecialization.h"
#include "HungerTraitSystemSpecialization.h"
#include "IntelligenceTraitSystemSpecialization.h"
#include "SpeedTraitSystemSpecialization.h"
#include "StaminaTraitSystemSpecialization.h"
#include "StrengthTraitSystemSpecialization.h"

constexpr float MIN_TRAIT_VALUE = 0.02f;
constexpr float MAX_TRAIT_VALUE = 1.0f;
constexpr float VARIANCE = 0.05f;

float mutateTrait(float trait, float r)
{
	float value = trait + r;
	value = std::max(value, MIN_TRAIT_VALUE);
	value = std::min(value, MAX_TRAIT_VALUE);
	return value;
}

void GeneSystem::mutate(GeneComponent& gene, SolengineV2::RandomNumberGenerator& rng) const
{
	AggressionTraitComponent& aggressionTrait = Gene::System::getAggressionTrait(gene);
	AggressionTrait::System::setTraitValue(aggressionTrait, mutateTrait(AggressionTrait::System::getTraitValue(aggressionTrait), rng.getRandom(-VARIANCE, VARIANCE)));

	DietTraitComponent& dietTrait = Gene::System::getDietTrait(gene);
	DietTrait::System::setTraitValue(dietTrait, mutateTrait(DietTrait::System::getTraitValue(dietTrait), rng.getRandom(-VARIANCE, VARIANCE)));
	
	HealthTraitComponent& healthTrait = Gene::System::getHealthTrait(gene);
	HealthTrait::System::setTraitValue(healthTrait, mutateTrait(HealthTrait::System::getTraitValue(healthTrait), rng.getRandom(-VARIANCE, VARIANCE)));
	
	IntelligenceTraitComponent& intelligenceTrait = Gene::System::getIntelligenceTrait(gene);
	IntelligenceTrait::System::setTraitValue(intelligenceTrait, mutateTrait(IntelligenceTrait::System::getTraitValue(intelligenceTrait), rng.getRandom(-VARIANCE, VARIANCE)));

	SpeedTraitComponent& speedTrait = Gene::System::getSpeedTrait(gene);
	SpeedTrait::System::setTraitValue(speedTrait, mutateTrait(SpeedTrait::System::getTraitValue(speedTrait), rng.getRandom(-VARIANCE, VARIANCE)));

	StaminaTraitComponent& staminaTrait = Gene::System::getStaminaTrait(gene);
	StaminaTrait::System::setTraitValue(staminaTrait, mutateTrait(StaminaTrait::System::getTraitValue(staminaTrait), rng.getRandom(-VARIANCE, VARIANCE)));

	StrengthTraitComponent& strengthTrait = Gene::System::getStrengthTrait(gene);
	StrengthTrait::System::setTraitValue(strengthTrait, mutateTrait(StrengthTrait::System::getTraitValue(strengthTrait), rng.getRandom(-VARIANCE, VARIANCE)));

	HungerTraitComponent& hungerTrait = Gene::System::getHungerTrait(gene);
	HungerTrait::System::setTraitValue(hungerTrait,
		(
			SpeedTrait::System::getTraitValue(speedTrait) +
			StaminaTrait::System::getTraitValue(staminaTrait) +
			StrengthTrait::System::getTraitValue(strengthTrait)
		) / 3);


	Gene::System::updateFamilyHistory(gene);
}

void GeneSystem::setAllTraits(GeneComponent& gene, const float set) const
{
	AggressionTrait::System::setTraitValue(Gene::System::getAggressionTrait(gene), set);
	DietTrait::System::setTraitValue(Gene::System::getDietTrait(gene), set);
	HealthTrait::System::setTraitValue(Gene::System::getHealthTrait(gene), set);
	HungerTrait::System::setTraitValue(Gene::System::getHungerTrait(gene), set);
	IntelligenceTrait::System::setTraitValue(Gene::System::getIntelligenceTrait(gene), set);
	SpeedTrait::System::setTraitValue(Gene::System::getSpeedTrait(gene), set);
	StaminaTrait::System::setTraitValue(Gene::System::getStaminaTrait(gene), set);
	StrengthTrait::System::setTraitValue(Gene::System::getStrengthTrait(gene), set);
}

void GeneSystem::updateFamilyHistory(GeneComponent& gene) const
{
	static int guid{ 0 };
	guid += 1;
	auto& familyHistory = Gene::System::getFamilyHistory(gene);
	familyHistory.data[0] = familyHistory.data[1];
	familyHistory.data[1] = familyHistory.data[2];
	familyHistory.data[2] = guid;
}

bool GeneSystem::isSharedFamilyHistory(const GeneComponent& geneA, const GeneComponent& geneB) const
{
	const auto& familyHistoryA = Gene::System::getFamilyHistory(geneA);
	const auto& familyHistoryB = Gene::System::getFamilyHistory(geneB);

	if (
		familyHistoryA.data[2] == familyHistoryB.data[2] ||
		familyHistoryA.data[1] == familyHistoryB.data[1] ||
		familyHistoryA.data[0] == familyHistoryB.data[0])
		return true;
	else return false;
}