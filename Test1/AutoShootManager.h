#pragma once
#include <vector>
#include "TransformManager.h"
#include "BulletCreationData.h"
#include <glm\geometric.hpp>
#include "Team.h"

struct AutoShootComponent
{
	AutoShootComponent(std::shared_ptr<TransformComponent> TC, Team t, glm::vec2 a) : 
		transformCo(TC), 
		team(t), 
		aim(a), 
		cooldown(0)
	{}

	std::shared_ptr<TransformComponent> transformCo;

	glm::vec2 aim;
	int cooldown;
	Team team;
};

class AutoShootManager
{
	TransformManager* TM;

public:
	AutoShootManager(TransformManager* transManager) : TM(transManager) { }

	std::vector<AutoShootComponent> AutoShooters{};
	std::vector<BulletCreationData> NewBullets{};

	void AddComponent()
	{
		AutoShooters.push_back(AutoShootComponent(TM->GetLastTransform(), Team::NONE, glm::normalize(glm::vec2{ -1.0f, 1.0f })));
	}
	void AddComponent(Team team, glm::vec2 aim)
	{
		AutoShooters.push_back(AutoShootComponent(TM->GetLastTransform(), team, aim));
	}

	void Process(int dt)
	{
		for (auto it = AutoShooters.begin(); it != AutoShooters.end();)
		{
			if (TM->IsToBeDeleted(it->transformCo.get()))
			{
				it = AutoShooters.erase(it);
			}
			else
			{
				updateComponent(it, dt);
				++it;
			}
		}
	}

private:
	void updateComponent(std::vector<AutoShootComponent>::iterator it, int dt)
	{
		if (it->cooldown > 0)
		{
			it->cooldown -= dt;
			if (it->cooldown <= 0)
			{
				it->cooldown == 0;
			}
		}
		else
		{
			glm::vec3 spawn{ it->transformCo->Pos.x + it->aim.x * (it->transformCo->Dims.x),
							 it->transformCo->Pos.y + it->aim.y * (it->transformCo->Dims.y),
							 0.0f };
			NewBullets.push_back(BulletCreationData(spawn, it->aim, 0.05f, it->team));
			it->cooldown = 4000;
		}
	}
};