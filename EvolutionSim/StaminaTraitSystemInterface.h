#pragma once
#include "StaminaTraitComponent.h"
class GeneComponent;

template <class Implementation>
class StaminaTraitSystemInterface
{
public:
	static void setTraitValue(StaminaTraitComponent& component, const float set) { component.value = set; }
	static float getTraitValue(const StaminaTraitComponent& component) { return component.value; }

	static void setEnergy(StaminaTraitComponent& component, const float set) { component.energy = set; }
	static float getEnergy(const StaminaTraitComponent& component) { return component.energy; }

	static float getMaxEnergy(const StaminaTraitComponent& component)
	{
		static Implementation system;
		return system.getMaxEnergy(component);
	}

	static void setEnergyToFull(StaminaTraitComponent& component)
	{
		static Implementation system;
		system.setEnergyToFull(component);
	}

	static void depleteEnergy(GeneComponent& gene, const float depletion)
	{
		static Implementation system;
		system.depleteEnergy(gene, depletion);
	}
};