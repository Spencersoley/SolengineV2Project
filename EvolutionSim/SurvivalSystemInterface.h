#pragma once
#include <chrono>
#include "SurvivalComponent.h"
struct GameData;

template <class Implementation>
class SurvivalSystemInterface
{
public:
	static SurvivalComponent::SurvivalState getSurvivalState(const SurvivalComponent& component) { return component.state; }
	static void setSurvivalState(SurvivalComponent& component, const SurvivalComponent::SurvivalState set) { component.state = set; }
	
	static bool getIsPoisoned(const SurvivalComponent& component) { return component.isPoisoned; }
	static void setIsPoisoned(SurvivalComponent& component, bool set) { component.isPoisoned = set; }

	static void update(const std::chrono::microseconds& dt, GameData& gameData)
	{
		static Implementation system;
		system.update(dt, gameData);
	}
};