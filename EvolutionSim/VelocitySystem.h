#pragma once
#include <vector>
#include "TransformSystem.h"
#include "../deps/include/glm/ext/vector_float2.hpp"

class VelocityComponent
{
private:
	friend class VelocitySystem;
	VelocityComponent(std::shared_ptr<TransformComponent> _transform, float _velocity, glm::vec2 _direction) 
		: 
		transform(_transform), 
		velocity(_velocity), 
		direction(_direction) 
	{}

	std::shared_ptr<TransformComponent> transform;
	float velocity;
	glm::vec2 direction;
};

class VelocitySystem 
{
	TransformSystem* transformSystem;

	std::map<int, VelocityComponent> velocityMap{};

public:
	VelocitySystem(TransformSystem* _transformSystem) 
		: 
		transformSystem(_transformSystem)
	{}


	void AddComponent(int handle, float velocity, glm::vec2 dir)
	{
		velocityMap.try_emplace(handle, VelocityComponent(transformSystem->GetLast(), velocity, dir));
	}

	void Process(int dt)
	{
		if (velocityMap.empty()) return;
		// something something spatial partitioning
		for (auto it = velocityMap.begin(); it != velocityMap.end(); ++it)
		{
			if (it->second.velocity != 0)
			{
				transformSystem->Translate(it->second.transform.get(), (it->second.direction * (it->second.velocity * dt)));
			}
		}
	}

	void SetVelocityAndDirection(int handle, float vel, glm::vec2 dir)
	{
		for (auto it = velocityMap.find(handle); it != velocityMap.end(); it = velocityMap.end())
		{
			it->second.velocity = vel;
			it->second.direction = dir;
		}
	}

	void SetVelocity(int handle, float vel)
	{
		for (auto it = velocityMap.find(handle); it != velocityMap.end(); it = velocityMap.end())
		{
			it->second.velocity = vel;
		}
	}

	void SetDirection(int handle, glm::vec2 dir)
	{
		for (auto it = velocityMap.find(handle); it != velocityMap.end(); it = velocityMap.end())
		{
			it->second.direction = dir;
		}
	}

	void DeleteComponent(int handle)
	{
		velocityMap.erase(handle);
	}
};