#pragma once
#include "VelocityComponent.h"
#include <glm\ext\vector_float2.hpp>

class GenerationManager;
class BeingManager;

template <class Implementation>
class VelocitySystemInterface
{
public:
	static void setVelocity(VelocityComponent& component, const float set) { component.velocity = set; }
	static void setDirection(VelocityComponent& component, const glm::vec2& set) { component.direction = set; }
	static float getVelocity(const VelocityComponent& component) { return component.velocity; }
	static const glm::vec2& getDirection(const VelocityComponent& component) { return component.direction; }

	static void update(BeingManager& beings, const GenerationManager& generationManager, unsigned int dt)
	{
		static Implementation system;
		system.update(beings, generationManager, dt);
	}
};