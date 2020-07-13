#include "VelocitySystem.h"
#include "BeingManager.h"
#include "TransformSystem.h"

constexpr float VELOCITY_MODIFIER = 0.000000005f;

void VelocitySystem::process(BeingManager& beings, long long dt) const
{
	//likely a conversion warning: it's not an issue?
	dt *= physicsSpeed;

	for (uint32_t beingHandle = 0; beingHandle < beings.getSize(); beingHandle++)
	{
		const VelocityComponent& component = beings.getVelocityComponent(beingHandle);
		if (component.velocity != 0)
		{
			transformSystem.translate(beings.getTransformComponent(beingHandle), (component.velocity * dt * VELOCITY_MODIFIER) * component.direction);
		}
	}
};
