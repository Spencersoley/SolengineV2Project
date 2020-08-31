#pragma once
#include "HealthTraitSystemSpecialization.h"
#include <algorithm>

constexpr float HEALTH_VALUE_MULTIPLIER = 100.0f;

void HealthTraitSystem::depleteCurrentHealth(HealthTraitComponent& component, const float deplete) const
{
	component.currentHealth = std::max(0.0f, component.currentHealth - deplete);
}

void HealthTraitSystem::setCurrentHealthToMax(HealthTraitComponent& component) const
{
	component.currentHealth = getHealthWhenFull(component);
}

float HealthTraitSystem::getHealthWhenFull(const HealthTraitComponent& component) const
{
	return HealthTrait::System::getTraitValue(component) * HEALTH_VALUE_MULTIPLIER;
}