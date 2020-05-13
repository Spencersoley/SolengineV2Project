#pragma once
#include <map>
#include <glm\geometric.hpp>
#include "TransformSystem.h"

class FoodComponent
{
	friend class FoodSystem;
	FoodComponent(std::shared_ptr<TransformComponent> transform)
		:
		transform(transform)
	{}

	std::shared_ptr<TransformComponent> transform;
};

class FoodSystem
{
	TransformSystem* transformSystem;
	std::map<int, FoodComponent> foodMap{};

public:
	FoodSystem(TransformSystem* _transformSystem)
		:
		transformSystem(_transformSystem)
	{}

	void AddComponent(int handle) 
	{ 
		foodMap.try_emplace(handle, FoodComponent(transformSystem->GetLast()));
	}

	glm::vec3 FindNearestFoodToPoint(const glm::vec3& point) const
	{
		if (foodMap.empty()) return point;

		auto it = foodMap.begin();
		glm::vec3 foodPos = transformSystem->GetPos(it->second.transform.get());
		glm::vec3 nearest = foodPos;

		float xDist = foodPos.x - point.x;
		float yDist = foodPos.y - point.y;
		float totalDist = xDist * xDist + yDist * yDist;

		float closestDist = totalDist;
		++it;

		while (it != foodMap.end())
		{
			foodPos = transformSystem->GetPos(it->second.transform.get());

			float xDist = foodPos.x - point.x;
			float yDist = foodPos.y - point.y;
			float totalDist = xDist * xDist + yDist * yDist;

			if (totalDist < closestDist)
			{
				closestDist = totalDist;
				nearest = foodPos;
			}
			++it;
		}

		return nearest;
	}

	bool NoFood() const
	{
		return foodMap.begin() == foodMap.end();
	}

	void DeleteComponent(int handle)
	{
		foodMap.erase(handle);
	}
};
