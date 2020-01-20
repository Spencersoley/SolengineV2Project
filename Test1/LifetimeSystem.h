#pragma once
#include "TransformSystem.h"

struct LifetimeComponent
{
	LifetimeComponent(int lifetime) :  Lifetime(lifetime) {}

	int Lifetime;
};

class LifetimeSystem
{
	TransformSystem* TM;

public:
	LifetimeSystem(TransformSystem* transformManager) : TM(transformManager) {}
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
				TM->SetToBeDeleted(it->first);
			}
		}
	}

	void DeleteComponent(int handle)
	{
		Lifetimes.erase(handle);
	}
};