#pragma once
#include <vector>
#include "TransformSystem.h"

class TargetableComponent
{
	friend class TargetableSystem;
	TargetableComponent(std::shared_ptr<TransformComponent> transform, Team team) 
		: 
	    Transform(transform), 
	    Team(team) 
	{}

	std::shared_ptr<TransformComponent> Transform;
	Team Team;
};

class TargetableSystem 
{
	TransformSystem* TS;

public:
	TargetableSystem(TransformSystem* transManager) 
		: 
		TS(transManager) 
	{}

	std::map<int, TargetableComponent> Targetables{};

	void AddComponent(int handle, Team team)
	{
		Targetables.insert(std::pair<int, TargetableComponent>(handle, TargetableComponent(TS->GetLast(), team)));
	}

	Team GetTeam(const TargetableComponent& targetable)
	{
		return targetable.Team;
	}

	Team GetTeam(const TargetableComponent* targetable)
	{
		return targetable->Team;
	}

	std::map<int, std::map<int, std::map<int, TargetableComponent*>>> GetSpatiallyPartitionedTargetables(int gridSize)
	{
		std::map<int, std::map<int, std::map<int, TargetableComponent*>>> targetableGrid;
		for (auto it = Targetables.begin(); it != Targetables.end(); ++it)
		{
			TransformComponent* TC = it->second.Transform.get();
			glm::vec3 pos = TS->GetPos(TC);
			targetableGrid[pos.x / gridSize][pos.y / gridSize][it->first] = &(it->second);
		}

		return targetableGrid;
	}

	glm::vec3 GetPos(const TargetableComponent& targetable)
	{
		return TS->GetPos(targetable.Transform.get());
	}

	glm::vec3 GetPos(const TargetableComponent* targetable)
	{
		return TS->GetPos(targetable->Transform.get());
	}

	void DeleteComponent(int handle)
	{
		Targetables.erase(handle);
	}
};