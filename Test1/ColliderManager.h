#pragma once
#include <vector>
#include "TransformManager.h"
#include "ColliderComponent.h"

class ColliderManager
{
public:
	ColliderManager(TransformManager* transManager) : TM(transManager) { }
	TransformManager* TM;
	std::vector<ColliderComponent> Colliders{};

	void Process()
	{
		if (Colliders.empty()) return;
		// something something spatial partitioning
		for (auto it = Colliders.begin(); it != Colliders.end(); ++it)
		{
			for (auto j = it; j != Colliders.end(); j++)
			{
				if (j == it) continue;
				//if (TM->IsToBeDeleted(j->transformCo.get())) continue;
				resolveCircularCollision(it->transformCo.get(), j->transformCo.get());
			}
		}

		for (auto it = Colliders.begin(); it != Colliders.end(); )
		{
			if (TM->IsToBeDeleted(it->transformCo.get()))
			{
				std::cout << "COLM-----------USE COUNT: " << it->transformCo.use_count() << std::endl;
				it = Colliders.erase(it);
				std::cout << "Colliders size: " << Colliders.size() << std::endl;
			}
			else
			{
				++it;
			}
		}
		//if (!Colliders.empty())
		//{
		//	auto it = --Colliders.end();
		//	if (TM->IsToBeDeleted(it->transformCo.get()))
		//	{
		//		std::cout << "--------------USE COUNT: " << it->transformCo.use_count() << std::endl;
		//		Colliders.erase(it);
		//		std::cout << "Colliders size: " << Colliders.size() << std::endl;
		//	}
		//}
	}

	void AddComponent()
	{
		Colliders.push_back(TM->GetLastTransform());
	}

private:

	void resolveCircularCollision(TransformComponent* a, TransformComponent* b)
	{
		glm::vec3 posA = a->Pos;
		glm::vec2 dimsA = a->Dims;
		glm::vec3 posB = b->Pos;
		glm::vec2 dimsB = b->Dims;

		glm::vec2 distVec = { posA.x - posB.x, posA.y - posB.y };
		float distance = glm::length(distVec);
		float MIN_DISTANCE = (dimsA.x + dimsA.y + dimsB.x + dimsB.y) / 4;
		float collisionDepth = MIN_DISTANCE - distance;

		if (collisionDepth > 0) // Collision
		{
			glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth * 0.5f;
			TM->Translate(a, collisionDepthVec);
			TM->Translate(b, -collisionDepthVec);

			TM->SetToBeDeleted(a);
			TM->SetToBeDeleted(b);
		}
	}
};