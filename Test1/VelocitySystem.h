#pragma once
#include <vector>
#include "TransformSystem.h"

class VelocityComponent
{
private:
	friend class VelocitySystem;
	VelocityComponent(std::shared_ptr<TransformComponent> transform, float velocity, glm::vec2 direction) : Transform(transform), Velocity(velocity), Direction(direction) {}

	std::shared_ptr<TransformComponent> Transform;
	float Velocity;
	glm::vec2 Direction;
};

class VelocitySystem : public System
{
	TransformSystem* TS;

public:
	VelocitySystem(TransformSystem* ts) : TS(ts) { }
	std::map<int, VelocityComponent> Velocities{};

	void AddComponent(int handle, float velocity, glm::vec2 dir)
	{
		Velocities.insert(std::pair<int, VelocityComponent>(handle, VelocityComponent(TS->GetLast(), velocity, dir)));
	}

	void Process(int dt)
	{
		if (Velocities.empty()) return;
		// something something spatial partitioning
		for (auto it = Velocities.begin(); it != Velocities.end(); ++it)
		{
			if (it->second.Velocity != 0)
			{
				TS->Translate(it->second.Transform.get(), (it->second.Direction * (it->second.Velocity * dt)));
			}
		}
	}

	void DeleteComponent(int handle)
	{
		Velocities.erase(handle);
	}
};