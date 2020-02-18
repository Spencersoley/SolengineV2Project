#pragma once
#include <vector>
#include "TransformSystem.h"
#include "SurvivalSystem.h"
#include <glm\geometric.hpp>

class ColliderComponent
{
private:
	friend class ColliderSystem;
	ColliderComponent(std::shared_ptr<TransformComponent> transform = nullptr)
		: 
		transform(transform)
	{}

	std::shared_ptr<TransformComponent> transform;
};

enum class ColliderType : unsigned int { FOOD = 1, CIRCULAR = 2 };

class ColliderSystem 
{
	TransformSystem* transformSystem;
	SurvivalSystem* survivalSystem;

	std::map<ColliderType, std::map<int, ColliderComponent>> colliderMap;

public:
	ColliderSystem(TransformSystem* _transformSystem, SurvivalSystem* _survivalSystem) 
		: 
		transformSystem(_transformSystem), 
		survivalSystem(_survivalSystem) 
	{}

	void AddComponent(int handle)
	{
		AddComponent(handle, ColliderType::CIRCULAR);
	}

	void AddComponent(int handle, ColliderType colliderType)
	{
		colliderMap[colliderType].try_emplace(handle, ColliderComponent(transformSystem->GetLast()));
	}

	void Process()
	{
		const int boundaryRadius = 400;

		// partition entities into their respective grids
		std::map<int, std::map<int, std::map<int, ColliderComponent*>>> colliderGrid; // x, y, id
		std::map<int, std::map<int, std::map<int, ColliderComponent*>>> foodColliderGrid; // x, y, id

		const int gridSize = 50; // this must be larger than the diameter of the largest object
		for (auto it = colliderMap[ColliderType::CIRCULAR].begin(); it != colliderMap[ColliderType::CIRCULAR].end(); ++it)
		{
			TransformComponent* transformComponent = it->second.transform.get();
			glm::vec3 pos = transformSystem->GetPos(transformComponent);
			colliderGrid[pos.x / gridSize][pos.y / gridSize][it->first] = &(it->second);
		
		}

		for (auto it = colliderMap[ColliderType::FOOD].begin(); it != colliderMap[ColliderType::FOOD].end(); ++it)
		{
			TransformComponent* transformComponent = it->second.transform.get();
			glm::vec3 pos = transformSystem->GetPos(transformComponent);
			foodColliderGrid[pos.x / gridSize][pos.y / gridSize][it->first] = &(it->second);
		}

		// for each bucket, see if elements collides with W, NW, N buckets
		for (std::map<int, std::map<int, std::map<int, ColliderComponent*>>>::iterator xt = colliderGrid.begin(); xt != colliderGrid.end(); ++xt) // map of possible x coords
		{
			for (std::map<int, std::map<int, ColliderComponent*>>::iterator yt = xt->second.begin(); yt != xt->second.end(); ++yt) // map of y coord for each x coord
			{
				auto nwItEnd = colliderGrid[xt->first - 1][yt->first - 1].end(); // grab north-west cell's end iterator
				auto nItEnd = colliderGrid[xt->first][yt->first - 1].end();      // grab north cell's end iterator
				auto wItEnd = colliderGrid[xt->first - 1][yt->first].end();      // grab west cell's end iterator

				auto foodItEnd = foodColliderGrid[xt->first][yt->first].end();
				auto nwFoodItEnd = foodColliderGrid[xt->first - 1][yt->first - 1].end();
				auto nFoodItEnd = foodColliderGrid[xt->first][yt->first - 1].end();
				auto wFoodItEnd = foodColliderGrid[xt->first - 1][yt->first].end();

				for (std::map<int, ColliderComponent*>::iterator bucketIt = yt->second.begin(); bucketIt != yt->second.end(); ++bucketIt) // map of items in each y in each x
				{
					auto nwIt = colliderGrid[xt->first - 1][yt->first - 1].begin();
					auto nIt = colliderGrid[xt->first][yt->first - 1].begin();
					auto wIt = colliderGrid[xt->first - 1][yt->first].begin();

					auto foodIt = foodColliderGrid[xt->first][yt->first].begin();
					auto nwFoodIt = foodColliderGrid[xt->first - 1][yt->first - 1].begin();
					auto nFoodIt = foodColliderGrid[xt->first][yt->first - 1].begin();
					auto wFoodIt = foodColliderGrid[xt->first - 1][yt->first].begin();

					for (auto innerIt = bucketIt; innerIt != yt->second.end(); ++innerIt)
					{
						if (abs(yt->first) + abs(xt->first) >= boundaryRadius / gridSize)
						{
							(transformSystem->GetDistFromCentre(innerIt->second->transform.get()) > boundaryRadius)
								? survivalSystem->SetIsHome(innerIt->first, true)
								: survivalSystem->SetIsHome(innerIt->first, false);
						}
						else survivalSystem->SetIsHome(innerIt->first, false);

						if (innerIt == bucketIt) continue;

						resolveCircularCollision(innerIt, bucketIt);
					}

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

					while (nwFoodIt != nwFoodItEnd)
					{
						resolveBeingCollidingWithFood(bucketIt, nwFoodIt);
						++nwFoodIt;
					}

					while (nFoodIt != nFoodItEnd)
					{
						resolveBeingCollidingWithFood(bucketIt, nFoodIt); 
						++nFoodIt;
					}

					while (wFoodIt != wFoodItEnd)
					{
						resolveBeingCollidingWithFood(bucketIt, wFoodIt); 
						++wFoodIt;
					}

					while (foodIt != foodItEnd)
					{
						resolveBeingCollidingWithFood(bucketIt, foodIt); 
						++foodIt;
					}
				}
			}
		}

	
	}

	void DeleteComponent(int handle)
	{
		colliderMap[ColliderType::CIRCULAR].erase(handle);
		colliderMap[ColliderType::FOOD].erase(handle);
	}

private:
	void resolveCircularCollision(std::map<int, ColliderComponent*>::iterator a, std::map<int, ColliderComponent*>::iterator b) // CIRCULAR -> CIRCULAR
	{
		TransformComponent* aTC = a->second->transform.get();
		TransformComponent* bTC = b->second->transform.get();

		glm::vec3 posA = transformSystem->GetPos(aTC);
		glm::vec2 dimsA = transformSystem->GetDims(aTC);
		glm::vec3 posB = transformSystem->GetPos(bTC);
		glm::vec2 dimsB = transformSystem->GetDims(bTC);

		glm::vec2 distVec = { posA.x - posB.x, posA.y - posB.y };
		float distance = glm::length(distVec);
		float minDistance = (dimsA.x + dimsA.y + dimsB.x + dimsB.y) / 4.0f;
		float collisionDepth = minDistance - distance;

		if (collisionDepth > 0) // Collision
		{
			if (distVec == glm::vec2{ 0.0f, 0.0f })
			{
				transformSystem->Translate(aTC, { 0.01f, 0.0f });
				transformSystem->Translate(bTC, { -0.01f, 0.0f });
			}
			else
			{
				glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth * 0.5f;
				transformSystem->Translate(aTC, collisionDepthVec);
				transformSystem->Translate(bTC, -collisionDepthVec);
			}
		}
	}

	void resolveBeingCollidingWithFood(std::map<int, ColliderComponent*>::iterator a, std::map<int, ColliderComponent*>::iterator b) // BEING -> FOOD
	{
		TransformComponent* aTC = a->second->transform.get();
		TransformComponent* bTC = b->second->transform.get();
		int aHandle = a->first;
		int bHandle = b->first;

		glm::vec3 posA = transformSystem->GetPos(aTC);
		glm::vec2 dimsA = transformSystem->GetDims(aTC) / 2.0f;

		glm::vec3 posB = transformSystem->GetPos(bTC); // a is the point, points can't collide with eachother
		
		// if A finds itself inside B
		if (posB.x < posA.x + dimsA.x
			&& posB.x > posA.x - dimsA.x
			&& posB.y < posA.y + dimsA.y
			&& posB.y > posA.y - dimsA.y)
		{
			survivalSystem->SetFed(aHandle, true);
			transformSystem->SetToBeDeleted(bHandle);
		}
	}
};