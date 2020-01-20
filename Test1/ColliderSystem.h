#pragma once
#include <vector>
#include "Team.h"
#include "TransformSystem.h"
#include "HealthSystem.h"

struct ColliderComponent
{
	ColliderComponent(std::shared_ptr<TransformComponent> transform, Team team) : Transform(transform), Team(team) {}

	std::shared_ptr<TransformComponent> Transform;
	Team Team;
};

struct PointColliderComponent
{
	PointColliderComponent(std::shared_ptr<TransformComponent> transform, Team team) : transform(transform), Team(team) {}

	std::shared_ptr<TransformComponent> transform;
	Team Team;
};

enum class ColliderType{ POINT, CIRCULAR };

class ColliderSystem
{
	TransformSystem* TS;
	HealthSystem* HS;

public:
	ColliderSystem(TransformSystem* ts, HealthSystem* hs) : TS(ts), HS(hs) {}
	std::map<int, ColliderComponent> Colliders{};
	std::map<int, PointColliderComponent> PointColliders{};

	void AddComponent(int handle)
	{
		AddComponent(handle, ColliderType::CIRCULAR, Team::NONE);
	}
	void AddComponent(int handle, ColliderType colType, Team team)
	{
		if (colType == ColliderType::POINT)
		{
			PointColliders.insert(std::pair<int, PointColliderComponent>(handle, PointColliderComponent(TS->GetLast(), team)));
		}
		if (colType == ColliderType::CIRCULAR)
		{
			Colliders.insert(std::pair<int, ColliderComponent>(handle, ColliderComponent(TS->GetLast(), team)));
		}
	}

	void Process()
	{
		//will it be faster doing a map conversion and allowing the collider to know their handle?
		//maybe

		for (auto it = Colliders.begin(); it != Colliders.end(); ++it)
		{
			for (auto j = it; j != Colliders.end(); j++)
			{
				if (j == it) continue;
				resolveCircularCollision(it, j);
			}
		}

		for (auto it = PointColliders.begin(); it != PointColliders.end(); ++it)
		{
			for (auto j = Colliders.begin(); j != Colliders.end(); j++)
			{
				resolvePointCollision(it, j);
			}
		}
	}

	void DeleteComponent(int handle)
	{
		Colliders.erase(handle);
		PointColliders.erase(handle);
	}

private:
	void resolveCircularCollision(std::map<int, ColliderComponent>::iterator a, std::map<int, ColliderComponent>::iterator b)
	{
		TransformComponent* aTC = a->second.Transform.get();
		TransformComponent* bTC = b->second.Transform.get();

		glm::vec3 posA = aTC->Pos;
		glm::vec2 dimsA = aTC->Dims;
		glm::vec3 posB = bTC->Pos;
		glm::vec2 dimsB = bTC->Dims;

		glm::vec2 distVec = { posA.x - posB.x, posA.y - posB.y };
		float distance = glm::length(distVec);
		float minDistance = (dimsA.x + dimsA.y + dimsB.x + dimsB.y) / 4.0f;
		float collisionDepth = minDistance - distance;

		if (collisionDepth > 0) // Collision
		{
			glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth * 0.5f;
			TS->Translate(aTC, collisionDepthVec);
			TS->Translate(bTC, -collisionDepthVec);

			//TM->SetToBeDeleted(a);
			//TM->SetToBeDeleted(b);
		}
	}

	void resolvePointCollision(std::map<int, PointColliderComponent>::iterator a, std::map<int, ColliderComponent>::iterator b)
	{
		Team aTeam = a->second.Team;
		Team bTeam = b->second.Team;

		if (aTeam == Team::NONE || bTeam == Team::NONE) {}
		else if (aTeam == bTeam) return;

		TransformComponent* aTC = a->second.transform.get();
		TransformComponent* bTC = b->second.Transform.get();
		int aHandle = a->first;
		int bHandle = b->first;

		glm::vec3 posA = aTC->Pos; // a is the point, points can't collide with eachother

		glm::vec3 posB = bTC->Pos;
		glm::vec2 dimsB = bTC->Dims;

		// if A finds itself inside B
		if (posA.x < posB.x + dimsB.x / 2.0f
			&& posA.x > posB.x - dimsB.x / 2.0f
			&& posA.y < posB.y + dimsB.y / 2.0f
			&& posA.y > posB.y - dimsB.y / 2.0f)
		{
			TS->SetToBeDeleted(aHandle);
			HS->UpdateHealth(bHandle, -10);
		}
	}
};