#pragma once
#include <glm\ext\vector_float2.hpp>

#include "VelocityComponent.h"

class BeingManager;
class TransformSystem;

class VelocitySystem
{
public:
	VelocitySystem(
		TransformSystem& tformSys
	) :
		transformSystem(tformSys)
	{}

	void update(BeingManager& beings, unsigned int dt) const;

	void setVelocity(VelocityComponent& component, const float set)       const;
	void setDirection(VelocityComponent& component, const glm::vec2& set)  const;

	float& getPhysicsSpeed() { return physicsSpeed; }
	float  getPhysicsSpeedVal() const { return physicsSpeed; }

private:
	TransformSystem& transformSystem;

	float physicsSpeed{ 10.0f };
};
