#pragma once
#include <vector>
#include "TransformManager.h"

struct LifetimeComponent
{
	LifetimeComponent(std::shared_ptr<TransformComponent> TC, int lt) : transformCo(TC), lifetime(lt) {}

	std::shared_ptr<TransformComponent> transformCo;
	int lifetime;
};

class LifetimeManager
{
	TransformManager* TM;

public:
	LifetimeManager(TransformManager* transManager) : TM(transManager) { }
	std::vector<LifetimeComponent> Lifetimes{};

	void AddComponent(int lifetime)
	{
		Lifetimes.push_back(LifetimeComponent(TM->GetLastTransform(), lifetime));
	}

	void Process(int dt)
	{
		for (auto it = Lifetimes.begin(); it != Lifetimes.end();)
		{
			if (TM->IsToBeDeleted(it->transformCo.get()))
			{
				it = Lifetimes.erase(it);
			}
			else
			{
				updateComponent(it, dt);
				++it;
			}
		}
	}

private:
	void updateComponent(std::vector<LifetimeComponent>::iterator it, int dt)
	{
		it->lifetime -= dt;

		if (it->lifetime <= 0)
		{
			TM->SetToBeDeleted(it->transformCo.get());
		}
	}
};