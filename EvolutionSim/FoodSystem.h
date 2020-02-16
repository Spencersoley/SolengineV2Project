#pragma once

#include <map>
#include "TransformSystem.h"
#include <glm\geometric.hpp>

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

	glm::vec3 FindNearestFoodToPoint(glm::vec3 point)
	{
		if (foodMap.empty()) return point;

		auto it = foodMap.begin();
		glm::vec3 foodPos = transformSystem->GetPos(it->second.transform.get());
		glm::vec3 nearest = foodPos;

		/*	auto dist = [foodPos, point]()
			{
				float xDist = foodPos.x - point.x;
				float yDist = foodPos.y - point.y;
				return xDist * xDist + yDist * yDist;
			};*/


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

	bool NoFood()
	{
		return (foodMap.begin() == foodMap.end());
	}

	void DeleteComponent(int handle)
	{
		foodMap.erase(handle);
	}
};
