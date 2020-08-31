#include <algorithm>
#include "StaminaTraitSystemSpecialization.h"
#include "GeneSystemSpecialization.h"
#include "GeneComponent.h"

constexpr float TRAIT_TO_FULL_ENERGY_CONVERSION = 1000.0f;

void StaminaTraitSystem::depleteEnergy(GeneComponent& gene, const float depletion) const
{
	StaminaTraitComponent& staminaComponent = Gene::System::getStaminaTrait(gene);
	float energy = std::max(StaminaTrait::System::getEnergy(staminaComponent) - depletion, 0.0f);

	StaminaTrait::System::setEnergy(staminaComponent, energy);
}

void StaminaTraitSystem::setEnergyToFull(StaminaTraitComponent& component) const
{
	StaminaTrait::System::setEnergy(component, StaminaTrait::System::getMaxEnergy(component));
}

float StaminaTraitSystem::getMaxEnergy(const StaminaTraitComponent& component) const
{
	return StaminaTrait::System::getTraitValue(component) * TRAIT_TO_FULL_ENERGY_CONVERSION;
}