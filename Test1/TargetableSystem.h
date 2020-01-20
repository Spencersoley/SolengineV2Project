#pragma once
#include <vector>
#include "TransformSystem.h"

struct TargetableComponent
{
	TargetableComponent(std::shared_ptr<TransformComponent> transform, Team team) : Transform(transform), Team(team) {}

	std::shared_ptr<TransformComponent> Transform;
	Team Team;
};

class TargetableSystem
{
	TransformSystem* TM;

public:
	TargetableSystem(TransformSystem* transManager) : TM(transManager) { }
	std::map<int, TargetableComponent> Targetables{};

	void AddComponent(int handle, Team team)
	{
		Targetables.insert(std::pair<int, TargetableComponent>(handle, TargetableComponent(TM->GetLast(), team)));
	}

	void DeleteComponent(int handle)
	{
		Targetables.erase(handle);
	}
};