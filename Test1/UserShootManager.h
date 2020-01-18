#pragma once
#include <vector>
#include "TransformManager.h"
#include "BulletCreationData.h"
#include <glm\geometric.hpp>
#include <InputManager.h>

struct UserShootComponent
{
	UserShootComponent(std::shared_ptr<TransformComponent> TC, Team t) : transformCo(TC), team(t), cooldown(0) {}

	std::shared_ptr<TransformComponent> transformCo;

	int cooldown;
	Team team;
};

class UserShootManager
{
	TransformManager* TM;
	CameraManager* CM;
	SolengineV2::InputManager* IM;

public:
	UserShootManager(TransformManager* transManager, CameraManager* camManager, SolengineV2::InputManager* inputManager) : 
		TM(transManager),
		CM(camManager),
		IM(inputManager) { }
	
	std::vector<UserShootComponent> UserShooters{};
	std::vector<BulletCreationData> NewBullets{};

	void AddComponent()
	{
		UserShooters.push_back(UserShootComponent(TM->GetLastTransform(), Team::NONE)); 
	}
	void AddComponent(Team team)
	{
		UserShooters.push_back(UserShootComponent(TM->GetLastTransform(), team));
	}

	void Process(int dt)
	{
		for (auto it = UserShooters.begin(); it != UserShooters.end();)
		{
			if (TM->IsToBeDeleted(it->transformCo.get()))
			{
				it = UserShooters.erase(it);
			}
			else
			{
				updateComponent(it, dt);
				++it;
			}
		}
	}

private:
	void updateComponent(std::vector<UserShootComponent>::iterator it, int dt)
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
			if (IM->KeyPress(SDL_BUTTON_LEFT))
			{
				glm::vec2 mouseCoords = CM->ScreenToWorldConvert(IM->GetMouseCoords());
				glm::vec2 pos{ it->transformCo->Pos.x, it->transformCo->Pos.y };
				glm::vec2 aim = glm::normalize(mouseCoords - pos);
				glm::vec2 dims = it->transformCo->Dims;

				glm::vec3 spawn{ pos.x + aim.x * dims.x,
								 pos.y + aim.y * dims.y,
								 0.0f };
				NewBullets.push_back(BulletCreationData(spawn, aim, 0.1f, it->team));
				it->cooldown = 30;
			}
		}
	}
};