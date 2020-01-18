#pragma once
#include <vector>
#include "TransformManager.h"

struct VelocityComponent
{
	VelocityComponent(std::shared_ptr<TransformComponent> TC, float vel, glm::vec2 dir) : transformCo(TC), velocity(vel), direction(dir) {}

	std::shared_ptr<TransformComponent> transformCo;
	float velocity;
	glm::vec2 direction;
};

class VelocityManager
{
	TransformManager* TM;

public:
	VelocityManager(TransformManager* transManager) : TM(transManager) { }	
	std::vector<VelocityComponent> Velocities{};

	void AddComponent(float velocity, glm::vec2 dir)
	{
		Velocities.push_back(VelocityComponent(TM->GetLastTransform(), velocity, dir));
	}

	void Process(int dt)
	{
		if (Velocities.empty()) return;
		// something something spatial partitioning
		for (auto it = Velocities.begin(); it != Velocities.end();)
		{
			TransformComponent* TC = it->transformCo.get();
			if (TM->IsToBeDeleted(TC))
			{
				it = Velocities.erase(it);
			}
			else
			{
				if (it->velocity != 0)
				{
					TM->Translate(TC, (it->direction * (it->velocity * dt))); 
				}

				++it;
			}
		}
	}
};