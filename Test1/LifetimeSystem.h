#pragma once
#include "TransformSystem.h"

class LifetimeComponent
{
private:
	friend class LifetimeSystem;
	LifetimeComponent(int lifetime) :  Lifetime(lifetime) {}

	int Lifetime;
};

class LifetimeSystem : public System
{
	TransformSystem* TS;

public:
	LifetimeSystem(TransformSystem* transformManager) : TS(transformManager) {}
	std::map<int, LifetimeComponent> Lifetimes{};

	void AddComponent(int handle, int lifetime)
	{
		Lifetimes.insert(std::pair<int, LifetimeComponent>(handle, LifetimeComponent(lifetime)));
	}

	void Process(int dt)
	{
		for (auto it = Lifetimes.begin(); it != Lifetimes.end(); ++it)
		{
			it->second.Lifetime -= dt;

			if (it->second.Lifetime <= 0)
			{
				TS->SetToBeDeleted(it->first);
			}
		}
	}

	void DeleteComponent(int handle)
	{
		Lifetimes.erase(handle);
	}
};