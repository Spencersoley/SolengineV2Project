#pragma once
#include <glm\ext\vector_float2.hpp>
#include <chrono>
#include "SpeedTraitComponent.h"
struct GameData;

template <class Implementation>
class SpeedTraitSystemInterface
{
public:
	static void setIsMoving(SpeedTraitComponent& component, const bool set) { component.isMoving = set; }
	static void setDirection(SpeedTraitComponent& component, const glm::vec2& set) { component.direction = set; }
	static void setTraitValue(SpeedTraitComponent& component, const float set) { component.value = set; }
	static bool getIsMoving(const SpeedTraitComponent& component) { return component.isMoving; }
	static const glm::vec2& getDirection(const SpeedTraitComponent& component) { return component.direction; }
	static float getTraitValue(const SpeedTraitComponent& component) { return component.value; }

	static void update(const std::chrono::microseconds& dt, GameData& gameData)
	{
		static Implementation system;
		system.update(dt, gameData);
	}
};