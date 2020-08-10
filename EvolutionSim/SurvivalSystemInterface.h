#pragma once

#include "SurvivalComponent.h"

class BeingManager;
class GenerationManager;
class TextureLibrary;

template <class Implementation>
class SurvivalSystemInterface
{
public:
	static bool getIsAlive(const SurvivalComponent& component) { return component.isAlive; }
	static SurvivalComponent::SurvivalState getSurvivalState(const SurvivalComponent& component) { return component.state; }
	static void setSurvivalState(SurvivalComponent& component, const SurvivalComponent::SurvivalState set) { component.state = set; }
	static void setIsAlive(SurvivalComponent& component, const bool set) { component.isAlive = set; }
	static void setFullness(SurvivalComponent& component, float set) { component.fullness = set; }
	static void setEnergy(SurvivalComponent& component, float set) { component.energy = set; }
	static float getFullness(const SurvivalComponent& component) { return component.fullness; }
	static float getEnergy(const SurvivalComponent& component) { return component.energy; }
	static void setStamina(SurvivalComponent& component, float set) { component.energy = set * 1000.0f; }

	static void update(BeingManager& beings, uint32_t dt, GenerationManager& generationManager, TextureLibrary& textureLibrary)
	{
		static Implementation system;
		system.update(beings, dt, generationManager, textureLibrary);
	}
};

