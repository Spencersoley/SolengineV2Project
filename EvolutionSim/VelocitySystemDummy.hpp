//#pragma once
//#include <glm\ext\vector_float2.hpp>
//
//#include "VelocityComponent.h"
//
//class BeingManager;
//
//class VelocitySystemDummy
//{
//public:
//	template <typename SystemManagerImplementation>
//	void update(BeingManager& beings, SystemManagerImplementation& systemManager, unsigned int dt) const;
//
//	void setVelocity(VelocityComponent& component, const float set)       const { component.velocity = set; }
//	void setDirection(VelocityComponent& component, const glm::vec2& set)  const { component.direction = set; }
//
//	float& getPhysicsSpeed() { return physicsSpeed; }
//	float  getPhysicsSpeedVal() const { return physicsSpeed; }
//
//private:
//	const float VELOCITY_MODIFIER = 0.00005f;
//	float physicsSpeed{ 10.0f };
//};
//
//template <typename SystemManagerImplementation>
//void VelocitySystemDummy::update(BeingManager& beings, SystemManagerImplementation& systemManager, unsigned int dt) const
//{
//	//likely a conversion warning: it's not an issue?
//	float adjustedDT = static_cast<int>(physicsSpeed) * dt * VELOCITY_MODIFIER;
//
//	const auto updatePosition = [&beings, adjustedDT, &systemManager](Being& being)
//	{
//		systemManager.transformSystem.translate(being.transform, (being.velocity.velocity * adjustedDT) * being.velocity.direction);
//	};
//
//	std::for_each(begin(beings.pool), end(beings.pool), updatePosition);
//}
//
