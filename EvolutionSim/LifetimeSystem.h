#pragma once
#include "TransformSystem.h"

class LifetimeComponent
{
private:
	friend class LifetimeSystem;
	LifetimeComponent(int lifetime) 
		:
		Lifetime(lifetime) 
	{}

	int Lifetime;
};

class LifetimeSystem 
{
	TransformSystem* transformSystem;
	std::map<int, LifetimeComponent> lifetimeMap{};

public:
	LifetimeSystem(TransformSystem* transformManager) 
		: 
		transformSystem(transformManager) 
	{}


	void AddComponent(int handle, int lifetime)
	{
		lifetimeMap.try_emplace(handle, LifetimeComponent(lifetime));
	}

	void Process(int dt)
	{
		for (auto it = lifetimeMap.begin(); it != lifetimeMap.end(); ++it)
		{
			it->second.Lifetime -= dt;

			if (it->second.Lifetime <= 0)
			{
				transformSystem->SetToBeDeleted(it->first);
			}
		}
	}

	void DeleteComponent(int handle)
	{
		lifetimeMap.erase(handle);
	}
};