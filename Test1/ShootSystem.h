#pragma once
#include <vector>
#include "BulletCreationData.h"
#include <glm\geometric.hpp>
#include "TransformSystem.h"
#include "TargetableSystem.h"
#include "CameraSystem.h"
#include <InputManager.h>

enum class ShootType { PLAYER, AUTO };

class ShootComponent : public System
{
private:
	friend class ShootSystem;
	ShootComponent(std::shared_ptr<TransformComponent> transform, ShootType shootType, Team team) :
		Transform(transform),
		ShootType(shootType),
		Team(team),
		Cooldown(0)
	{}

	std::shared_ptr<TransformComponent> Transform;
	int Cooldown;
	ShootType ShootType;
	Team Team;
};

class ShootSystem
{
	TransformSystem* TS;
	TargetableSystem* TGS;
	CameraSystem* CS;
	SolengineV2::InputManager* IM;

public:
	ShootSystem(TransformSystem* ts, TargetableSystem* tgs, CameraSystem* cs, SolengineV2::InputManager* im) : 
		TS(ts), 
		TGS(tgs),
		CS(cs),
		IM(im) { }

	std::map<int, ShootComponent> Shooters{};
	std::vector<BulletCreationData> NewBullets{};

	void AddComponent(int handle)
	{
		AddComponent(handle, ShootType::AUTO, Team::NONE);
	}
	void AddComponent(int handle, ShootType shootType, Team team)
	{
		Shooters.insert(std::pair<int, ShootComponent>(handle, ShootComponent(TS->GetLast(), shootType, team)));
	}

	void Process(int dt)
	{
		for (auto it = Shooters.begin(); it != Shooters.end(); ++it)
		{
			if (it->second.Cooldown > 0)
			{
				it->second.Cooldown -= dt;
				if (it->second.Cooldown <= 0)
				{
					it->second.Cooldown == 0;
				}
			}
			else
			{
				ShootType st = it->second.ShootType;
				glm::vec3 pos = TS->GetPos(it->second.Transform.get());
				glm::vec3 dims = TS->GetDims(it->second.Transform.get());
				Team team = it->second.Team;

				if (st == ShootType::PLAYER)
				{
					if (IM->KeyState(SDL_BUTTON_LEFT))
					{
						glm::vec2 aim = playerAim(pos);
						glm::vec3 spawn{ pos.x + aim.x * dims.x, pos.y + aim.y * dims.y, 0.0f };
						NewBullets.push_back(BulletCreationData(spawn, aim, 0.2f, team));
						it->second.Cooldown = 30;
					}
				}
				else if (st == ShootType::AUTO)
				{
					glm::vec2 aim = autoAim(pos, team);
					if (aim != glm::vec2{ 0.0f, 0.0f })
					{
						glm::vec3 spawn{ pos.x + aim.x * dims.x, pos.y + aim.y * dims.y, 0.0f };
						NewBullets.push_back(BulletCreationData(spawn, aim, 0.2f, team));
						it->second.Cooldown = 4000;
					}
					else
					{
						//notarget
					}
				}
			}
		}
	}

	void DeleteComponent(int handle)
	{
		auto it = Shooters.find(handle);
		if (it == Shooters.end()) return;
		Shooters.erase(handle);
	}

private:
	glm::vec2 autoAim(glm::vec3 pos, Team team)
	{
		glm::vec2 closestPos{ 0.0f, 0.0f };
		constexpr float MAX_DIST = std::numeric_limits<float>::max();
		float closestDist = MAX_DIST;
		for (const auto& targetable : TGS->Targetables)
		{
			if (TGS->GetTeam(targetable.second) != team)
			{
				glm::vec3 tarPos = TGS->GetPos(targetable.second);

				float xDist = tarPos.x - pos.x;
				float yDist = tarPos.y - pos.y;

				float tarDist = xDist * xDist + yDist * yDist;
				if (tarDist < closestDist)
				{
					closestPos = tarPos;
					closestDist = tarDist;
				}
			}
		}

		if (closestDist < MAX_DIST)
		{
			return glm::normalize(glm::vec2{ closestPos.x - pos.x, closestPos.y - pos.y });
		}
		else
		{
			return glm::vec2{ 0.0f, 0.0f };
		}
	}

	glm::vec2 playerAim(glm::vec3 pos)
	{
		glm::vec2 posvec2 = { pos.x, pos.y };
		return glm::normalize(CS->ScreenToWorldConvert(IM->GetMouseCoords()) - posvec2);
	}
};