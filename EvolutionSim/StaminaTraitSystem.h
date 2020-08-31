#pragma once
class GeneComponent;

class StaminaTraitSystem
{
public:
	void depleteEnergy(GeneComponent& gene, const float depletion) const;
	void setEnergyToFull(StaminaTraitComponent& component) const;
	float getMaxEnergy(const StaminaTraitComponent& component) const;
};