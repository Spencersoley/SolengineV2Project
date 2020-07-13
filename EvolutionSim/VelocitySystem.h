#pragma once
#include "VelocityComponent.h"
#include <glm\ext\vector_float2.hpp>

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

	void process(BeingManager& beings, long long dt) const;

	void setVelocity  (VelocityComponent& component, const float set)       const { component.velocity = set; }
	void setDirection (VelocityComponent& component, const glm::vec2& set)  const { component.direction = set; }

	float& getPhysicsSpeed    ()       { return physicsSpeed; }
	float  getPhysicsSpeedVal () const { return physicsSpeed; }

private:
	TransformSystem& transformSystem;

	float physicsSpeed{ 10.0f };
};
