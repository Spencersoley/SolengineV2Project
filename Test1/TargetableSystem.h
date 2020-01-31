#pragma once
#include <vector>
#include "TransformSystem.h"

class TargetableComponent
{
private:
	friend class TargetableSystem;
	TargetableComponent(std::shared_ptr<TransformComponent> transform, Team team) : Transform(transform), Team(team) {}

	std::shared_ptr<TransformComponent> Transform;
	Team Team;
};

class TargetableSystem : public System
{
	TransformSystem* TS;

public:
	TargetableSystem(TransformSystem* transManager) : TS(transManager) { }
	std::map<int, TargetableComponent> Targetables{};

	void AddComponent(int handle, Team team)
	{
		Targetables.insert(std::pair<int, TargetableComponent>(handle, TargetableComponent(TS->GetLast(), team)));
	}

	Team GetTeam(const TargetableComponent& targetable)
	{
		return targetable.Team;
	}

	glm::vec3 GetPos(const TargetableComponent& targetable)
	{
		return TS->GetPos(targetable.Transform.get());
	}

	void DeleteComponent(int handle)
	{
		Targetables.erase(handle);
	}
};