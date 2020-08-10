#include "BeingManager.h"
#include "GenerationManager.h"

#include "VelocitySystemImplementation.h"
#include "TransformSystemImplementation.h"
#include "GenerationSystemImplementation.h"

constexpr float VELOCITY_MODIFIER = 0.000005f;

void VelocitySystem::update(BeingManager& beings, const GenerationManager& generationManager, unsigned int dt) const
{
	//likely a conversion warning: it's not an issue?
	float physicsSpeed = Generation::System::getPhysicsSpeed(generationManager.component);
	float adjustedDT = physicsSpeed * dt * VELOCITY_MODIFIER;

	const auto updatePosition = [&beings, adjustedDT](Being& being)
	{
		Transform::System::translate(being.transform, Velocity::System::getVelocity(being.velocity) * adjustedDT * Velocity::System::getDirection(being.velocity));
	};

	std::for_each(begin(beings.pool), end(beings.pool), updatePosition);
}

