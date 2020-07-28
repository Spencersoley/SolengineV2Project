#include "BeingManager.h"
#include "TransformSystem.h"
#include "VelocitySystem.h"

constexpr float VELOCITY_MODIFIER = 0.000005f;

void VelocitySystem::update(BeingManager& beings, unsigned int dt) const
{
	//likely a conversion warning: it's not an issue?
	float adjustedDT = static_cast<int>(physicsSpeed) * dt * VELOCITY_MODIFIER;

	const auto updatePosition = [&beings, adjustedDT, this](Being& being)
	{
		transformSystem.translate(being.transform, (being.velocity.velocity * adjustedDT) * being.velocity.direction);
	};

	std::for_each(begin(beings.pool), end(beings.pool), updatePosition);
}

void VelocitySystem::setVelocity(VelocityComponent& component, const float set) const
{ 
	component.velocity = set; 
}

void VelocitySystem::setDirection(VelocityComponent& component, const glm::vec2& set) const
{
	component.direction = set;
}
