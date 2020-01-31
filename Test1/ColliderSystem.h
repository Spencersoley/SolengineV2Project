#pragma once
#include <vector>
#include "Team.h"
#include "TransformSystem.h"
#include "HealthSystem.h"
#include <glm\geometric.hpp>

class ColliderComponent
{
private:
	friend class ColliderSystem;
	ColliderComponent(std::shared_ptr<TransformComponent> transform, Team team) : Transform(transform), Team(team) {}

	std::shared_ptr<TransformComponent> Transform;
	Team Team;
};

class PointColliderComponent
{
private:
	friend class ColliderSystem;
	PointColliderComponent(std::shared_ptr<TransformComponent> transform, Team team) : Transform(transform), Team(team) {}

	std::shared_ptr<TransformComponent> Transform;
	Team Team;
};

enum class ColliderType { POINT, CIRCULAR };

class ColliderSystem : public System
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

	std::map<int, std::map<int, std::map<int, ColliderComponent*>>> colliderGrid;
	std::map<int, std::map<int, std::map<int, PointColliderComponent*>>> pointColliderGrid;


	void Process()
	{
		// partition entities
		colliderGrid.clear();
		const int gridSize = 40;
		for (auto it = Colliders.begin(); it != Colliders.end(); ++it)
		{
			TransformComponent* TC = it->second.Transform.get();
			glm::vec3 pos = TS->GetPos(TC);
			colliderGrid[pos.x / gridSize][pos.y / gridSize][it->first] = &(it->second);
		}
		pointColliderGrid.clear();

		for (auto it = PointColliders.begin(); it != PointColliders.end(); ++it)
		{
			TransformComponent* TC = it->second.Transform.get();
			glm::vec3 pos = TS->GetPos(TC);
			pointColliderGrid[pos.x / gridSize][pos.y / gridSize][it->first] = &(it->second);
		}

		// for each bucket, see if elements collides with W, NW, N buckets
		for (std::map<int, std::map<int, std::map<int, ColliderComponent*>>>::iterator xt = colliderGrid.begin(); xt != colliderGrid.end(); ++xt) // map of possible x coords
		{
			for (std::map<int, std::map<int, ColliderComponent*>>::iterator yt = xt->second.begin(); yt != xt->second.end(); ++yt) // map of y coord for each x coord
			{
				auto nwIt = colliderGrid[xt->first - 1][yt->first - 1].begin();
				auto nIt = colliderGrid[xt->first][yt->first - 1].begin();
				auto wIt = colliderGrid[xt->first - 1][yt->first].begin();
				auto nwItEnd = colliderGrid[xt->first - 1][yt->first - 1].end();
				auto nItEnd = colliderGrid[xt->first][yt->first - 1].end();
				auto wItEnd = colliderGrid[xt->first - 1][yt->first].end();


				auto pointItEnd = pointColliderGrid[xt->first][yt->first].end();
				auto nwPointItEnd = pointColliderGrid[xt->first - 1][yt->first - 1].end();
				auto nPointItEnd = pointColliderGrid[xt->first][yt->first - 1].end();
				auto wPointItEnd = pointColliderGrid[xt->first - 1][yt->first].end();

				for (std::map<int, ColliderComponent*>::iterator bucketIt = yt->second.begin(); bucketIt != yt->second.end(); ++bucketIt) // map of items in each y in each x
				{
					for (auto innerIt = bucketIt; innerIt != yt->second.end(); ++innerIt)
					{
						if (innerIt == bucketIt) continue;
						resolveCircularCollision(innerIt, bucketIt);
					}

					auto pointIt = pointColliderGrid[xt->first][yt->first].begin();
					auto nwPointIt = pointColliderGrid[xt->first - 1][yt->first - 1].begin();
					auto nPointIt = pointColliderGrid[xt->first][yt->first - 1].begin();
					auto wPointIt = pointColliderGrid[xt->first - 1][yt->first].begin();

					while (nwIt != nwItEnd)
					{
						resolveCircularCollision(bucketIt, nwIt);
						++nwIt;
					}

					while (nIt != nItEnd)
					{
						resolveCircularCollision(bucketIt, nIt);
						++nIt;
					}

					while (wIt != wItEnd)
					{
						resolveCircularCollision(bucketIt, wIt);
						++wIt;
					}

					while (nwPointIt != nwPointItEnd)
					{
						resolvePointCollision(bucketIt, nwPointIt); //could do with the handle..
						++nwPointIt;
					}

					while (nPointIt != nPointItEnd)
					{
						resolvePointCollision(bucketIt, nPointIt); //could do with the handle..
						++nPointIt;
					}

					while (wPointIt != wPointItEnd)
					{
						resolvePointCollision(bucketIt, wPointIt); //could do with the handle..
						++wPointIt;
					}

					while (pointIt != pointItEnd)
					{
						resolvePointCollision(bucketIt, pointIt); //could do with the handle..
						++pointIt;
					}
				}
			}
		}




		////will it be faster doing a map conversion and allowing the collider to know their handle?
		////maybe

		//for (auto it = Colliders.begin(); it != Colliders.end(); ++it)
		//{
		//	for (auto j = it; j != Colliders.end(); j++)
		//	{
		//		if (j == it) continue;
		//		resolveCircularCollision(it, j);
		//	}
		//}

		//for (auto it = PointColliders.begin(); it != PointColliders.end(); ++it)
		//{
		//	for (auto j = Colliders.begin(); j != Colliders.end(); j++)
		//	{
		//		resolvePointCollision(it, j);
		//	}
		//}
	}

	void DeleteComponent(int handle)
	{
		Colliders.erase(handle);
		PointColliders.erase(handle);
	}

private:
	void resolveCircularCollision(std::map<int, ColliderComponent*>::iterator a, std::map<int, ColliderComponent*>::iterator b)
	{
		TransformComponent* aTC = a->second->Transform.get();
		TransformComponent* bTC = b->second->Transform.get();

		glm::vec3 posA = TS->GetPos(aTC);
		glm::vec2 dimsA = TS->GetDims(aTC);
		glm::vec3 posB = TS->GetPos(bTC);
		glm::vec2 dimsB = TS->GetDims(bTC);

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

	void resolveCircularCollision(std::vector<ColliderComponent*>::iterator a, std::vector<ColliderComponent*>::iterator b)
	{
		TransformComponent* aTC = (*a)->Transform.get();
		TransformComponent* bTC = (*b)->Transform.get();

		glm::vec3 posA = TS->GetPos(aTC);
		glm::vec2 dimsA = TS->GetDims(aTC);
		glm::vec3 posB = TS->GetPos(bTC);
		glm::vec2 dimsB = TS->GetDims(bTC);

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

	void resolvePointCollision(std::map<int, ColliderComponent*>::iterator b, std::map<int, PointColliderComponent*>::iterator a)
	{
		Team aTeam = a->second->Team;
		Team bTeam = b->second->Team;

		if (aTeam == Team::NONE || bTeam == Team::NONE) {}
		else if (aTeam == bTeam) return;

		TransformComponent* aTC = a->second->Transform.get();
		TransformComponent* bTC = b->second->Transform.get();
		int aHandle = a->first;
		int bHandle = b->first;

		glm::vec3 posA = TS->GetPos(aTC); // a is the point, points can't collide with eachother

		glm::vec3 posB = TS->GetPos(bTC);
		glm::vec2 dimsB = TS->GetDims(bTC);

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

	void resolvePointCollision(std::map<int, PointColliderComponent>::iterator a, std::map<int, ColliderComponent>::iterator b)
	{
		Team aTeam = a->second.Team;
		Team bTeam = b->second.Team;

		if (aTeam == Team::NONE || bTeam == Team::NONE) {}
		else if (aTeam == bTeam) return;

		TransformComponent* aTC = a->second.Transform.get();
		TransformComponent* bTC = b->second.Transform.get();
		int aHandle = a->first;
		int bHandle = b->first;

		glm::vec3 posA = TS->GetPos(aTC); // a is the point, points can't collide with eachother

		glm::vec3 posB = TS->GetPos(bTC);
		glm::vec2 dimsB = TS->GetDims(bTC);

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