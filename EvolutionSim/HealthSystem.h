#pragma once
#include <map>
#include "TransformSystem.h"

class HealthComponent
{
private:
	friend class HealthSystem;
	HealthComponent(std::shared_ptr<TransformComponent> healthbar, int health, int healthbarHandle)
		:
		healthbarHandle(healthbarHandle),
		currentHealth(health),
		maximumHealth(health),
		healthbar(healthbar),
		regenerationRate(1.0f)
	{}

	std::shared_ptr<TransformComponent> healthbar;

	int healthbarHandle;
	int currentHealth;
	int maximumHealth;
	int regenerationRate;
};

class HealthSystem
{
	TransformSystem* transformSystem;
	std::map<int, HealthComponent> healthMap{};

public:
	HealthSystem(TransformSystem* _transformSystem)
		:
		transformSystem(_transformSystem)
	{}

	std::map<int, HealthComponent>::iterator GetHealthMapBegin() { return healthMap.begin(); }
	std::map<int, HealthComponent>::iterator GetHealthMapEnd() { return healthMap.end(); }

	void AddComponent(int handle, int healthbarHandle)
	{
		healthMap.try_emplace(handle, HealthComponent(transformSystem->GetLast(), 100, healthbarHandle));
	}

	void UpdateHealth(int handle, int healthChange) 
	{
		auto it = healthMap.find(handle);
		if (it == healthMap.end()) return;

		float oldHealth = it->second.currentHealth;

		float newHealth = oldHealth + healthChange;

		if (newHealth <= 0)
		{
			transformSystem->SetToBeDeleted(handle);
		}
		else
		{
			if (newHealth >= it->second.maximumHealth)
			{
				newHealth = it->second.maximumHealth;
			}
			it->second.currentHealth = newHealth;
			transformSystem->Slice(it->second.healthbar.get(), newHealth / oldHealth);
		}
	}

	int GetHealthbarHandle(HealthComponent* healthComponent) const { return healthComponent->healthbarHandle; }

	// Alternative: implement a conditional visibility component?
	TransformComponent* GetTransform(HealthComponent* HC) { return HC->healthbar.get(); }

	void DeleteComponent(int handle) { healthMap.erase(handle); }
};