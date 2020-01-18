#pragma once
#include <vector>
#include "TransformManager.h"
#include "Team.h"

struct ColliderComponent
{
	ColliderComponent(std::shared_ptr<TransformComponent> TC, Team t) : transformCo(TC), team(t) {}

	std::shared_ptr<TransformComponent> transformCo;
	Team team;
};

struct PointColliderComponent
{
	PointColliderComponent(std::shared_ptr<TransformComponent> TC, Team t) : transformCo(TC), team(t) {}

	std::shared_ptr<TransformComponent> transformCo;
	Team team;
};

enum class ColliderType{ POINT, CIRCULAR };

class ColliderManager
{
	TransformManager* TM;

public:
	ColliderManager(TransformManager* transManager) : TM(transManager) {}
	std::vector<ColliderComponent> Colliders{};
	std::vector<PointColliderComponent> PointColliders{};

	void Process()
	{
		for (auto it = Colliders.begin(); it != Colliders.end(); ++it)
		{
			for (auto j = it; j != Colliders.end(); j++)
			{
				if (j == it) continue;
				resolveCircularCollision(it->transformCo.get(), it->team, j->transformCo.get(), j->team);
			}
		}

		for (auto it = PointColliders.begin(); it != PointColliders.end(); ++it)
		{
			for (auto j = Colliders.begin(); j != Colliders.end(); j++)
			{
				resolvePointCollision(it->transformCo.get(), it->team, j->transformCo.get(), j->team);
			}
		}

		for (auto it = Colliders.begin(); it != Colliders.end(); )
		{
			if (TM->IsToBeDeleted(it->transformCo.get()))
			{
				it = Colliders.erase(it);
			}
			else
			{
				++it;
			}
		}

		for (auto it = PointColliders.begin(); it != PointColliders.end(); )
		{
			if (TM->IsToBeDeleted(it->transformCo.get()))
			{
				it = PointColliders.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void AddComponent()
	{
		Colliders.push_back(ColliderComponent(TM->GetLastTransform(), Team::NONE));
	}
	void AddComponent(ColliderType colType, Team team)
	{
		if (colType == ColliderType::POINT)
		{
			PointColliders.push_back(PointColliderComponent(TM->GetLastTransform(), team));
		}
		if (colType == ColliderType::CIRCULAR)
		{
			Colliders.push_back(ColliderComponent(TM->GetLastTransform(), team));
		}
	}

private:
	void resolveCircularCollision(TransformComponent* a, const Team aTeam, TransformComponent* b, const Team bTeam)
	{
		glm::vec3 posA = a->Pos;
		glm::vec2 dimsA = a->Dims;
		glm::vec3 posB = b->Pos;
		glm::vec2 dimsB = b->Dims;

		glm::vec2 distVec = { posA.x - posB.x, posA.y - posB.y };
		float distance = glm::length(distVec);
		float minDistance = (dimsA.x + dimsA.y + dimsB.x + dimsB.y) / 4.0f;
		float collisionDepth = minDistance - distance;

		if (collisionDepth > 0) // Collision
		{
			glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth * 0.5f;
			TM->Translate(a, collisionDepthVec);
			TM->Translate(b, -collisionDepthVec);

			//TM->SetToBeDeleted(a);
			//TM->SetToBeDeleted(b);
		}
	}

	void resolvePointCollision(TransformComponent* a, Team aTeam, TransformComponent* b, Team bTeam)
	{
		if (aTeam == Team::NONE || bTeam == Team::NONE) {}
		else if (aTeam == bTeam) return;

		glm::vec3 posA = a->Pos; // a is the point, points can't collide with eachother
 
		glm::vec3 posB = b->Pos;
		glm::vec2 dimsB = b->Dims;

		// if A finds itself inside B
		if (posA.x < posB.x + dimsB.x / 2.0f
			&& posA.x > posB.x - dimsB.x / 2.0f
			&& posA.y < posB.y + dimsB.y / 2.0f
	        && posA.y > posB.y - dimsB.y / 2.0f)
		{
			TM->SetToBeDeleted(a);
			TM->SetToBeDeleted(b);
		}
	}
};