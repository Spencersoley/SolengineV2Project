#pragma once
#include <map>
#include "TransformSystem.h"

class HealthComponent
{
private:
	friend class HealthSystem;
	HealthComponent(std::shared_ptr<TransformComponent> healthbarTransform, int health) : CurrentHealth(health), MaximumHealth(health), HealthbarTransform(healthbarTransform) {}

	std::shared_ptr<TransformComponent> HealthbarTransform; //should be the transform of the child entity

	int CurrentHealth;
	int MaximumHealth;
	int RegenerationRate = 1.0f;
};

class HealthSystem : public System
{
	TransformSystem* TS;

public:
	HealthSystem(TransformSystem* ts) : TS(ts) {}

	std::map<int, HealthComponent> Healths{};

	void AddComponent(int handle, int healthbarHandle)
	{
		Healths.insert(std::pair<int, HealthComponent>(handle, HealthComponent(TS->GetLast(), 100)));
	}

	void UpdateHealth(int handle, int healthChange)
	{
		auto it = Healths.find(handle);
		if (it == Healths.end()) return;

		float oldHealth = it->second.CurrentHealth;
	
		float newHealth = oldHealth + healthChange;

		if (newHealth <= 0)
		{
			TS->SetToBeDeleted(handle);
		}
		else 
		{
			if (newHealth >= it->second.MaximumHealth)
			{
				newHealth = it->second.MaximumHealth;
			}
			it->second.CurrentHealth = newHealth;
			TS->slice(it->second.HealthbarTransform.get(), newHealth / oldHealth);
		}
	}

	void DeleteComponent(int handle)
	{
		Healths.erase(handle);
	}
};