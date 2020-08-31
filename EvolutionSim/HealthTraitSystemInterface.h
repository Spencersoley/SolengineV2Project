#pragma once
#include "HealthTraitComponent.h"

template <class Implementation>
class HealthTraitSystemInterface
{
public:
	static void setTraitValue(HealthTraitComponent& component, const float set) { component.value = set; }
	static float getTraitValue(const HealthTraitComponent& component) { return component.value; }

	static void setCurrentHealth(HealthTraitComponent& component, const float set) { component.currentHealth = set; }
	static float getCurrentHealth(const HealthTraitComponent& component) { return component.currentHealth; }

	static bool getHasHealth(const HealthTraitComponent& component) { return component.currentHealth > 0.0f; }

	static void depleteCurrentHealth(HealthTraitComponent& component, const float deplete)
	{
		static Implementation system;
		system.depleteCurrentHealth(component, deplete);
	}

	static void setCurrentHealthToMax(HealthTraitComponent& component)
	{
		static Implementation system;
		system.setCurrentHealthToMax(component);
	}

	static float getHealthWhenFull(const HealthTraitComponent& component)
	{
		static Implementation system;
		return system.getHealthWhenFull(component);
	}
};
