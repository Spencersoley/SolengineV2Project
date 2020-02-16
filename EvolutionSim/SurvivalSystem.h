#pragma once

#include "TransformSystem.h"
#include "FoodSystem.h"
#include "VelocitySystem.h"
#include "GraphicsSystem.h"

class SurvivalComponent
{
private:
	friend class SurvivalSystem;
	SurvivalComponent(std::shared_ptr<TransformComponent> _transform)
		:
		transform(_transform),
		isFed(0),
		isHome(0)
	{}

	std::shared_ptr<TransformComponent> transform;
	bool isHome;   // collider system knows of survival system, thus it can set isHome on collision with home
	bool isFed;    // Collider system knows of survival system, thus it can set isFed on collision with food
};

class SurvivalSystem
{
	TransformSystem* transformSystem;
	FoodSystem* foodSystem;
	VelocitySystem* velocitySystem;
	GraphicsSystem* graphicsSystem;

	std::map<int, SurvivalComponent*> hungryMap{};
	std::map<int, SurvivalComponent*> homeseekingMap{};
	std::map<int, SurvivalComponent*> awaitingMap{};

public:
	SurvivalSystem(TransformSystem* _transformSystem, VelocitySystem* _velocitySystem, FoodSystem* _foodSystem, GraphicsSystem* _graphicsSystem)
		:
		transformSystem(_transformSystem),
		foodSystem(_foodSystem),
		velocitySystem(_velocitySystem),
		graphicsSystem(_graphicsSystem)
	{}
	~SurvivalSystem()
	{
		for (auto kvp : hungryMap)
		{
			delete kvp.second;
		}
		for (auto kvp : homeseekingMap)
		{
			delete kvp.second;
		}
		for (auto kvp : awaitingMap)
		{
			delete kvp.second;
		}
	}

	int shouldDuplicate = 3;

	void AddComponent(int handle)
	{
		SurvivalComponent* surv = new SurvivalComponent(transformSystem->GetLast());
		hungryMap.try_emplace(handle, surv);
	}

	void Process(int adjustedDeltaTicks)
	{
		bool noFood = foodSystem->NoFood();

		for (auto it = hungryMap.begin(); it != hungryMap.end(); )
		{
			if (it->second->isFed) //set in collider
			{
				homeseekingMap.insert({ it->first, it->second });
				SeekHome(it->first, transformSystem->GetPos(it->second->transform.get()), adjustedDeltaTicks); //sets off for home
				SetIsHome(it->first, false);
				graphicsSystem->SetColour(it->first, { 0, 255, 0, 255 });
				hungryMap.erase(it++);
			}
			else
			{
				SeekFood(it->first, transformSystem->GetPos(it->second->transform.get()), adjustedDeltaTicks); //repeatedly seeks nearest food
				if (noFood) graphicsSystem->SetColour(it->first, { 255, 0, 0, 255 });
				++it;
			}
		}

		for (auto it = homeseekingMap.begin(); it != homeseekingMap.end(); )
		{
			if (it->second->isHome) //set in collider
			{
				awaitingMap.insert({ it->first, it->second });
				velocitySystem->SetVelocity(it->first, 0.0f);
				graphicsSystem->SetColour(it->first, { 0, 255, 255, 255 });
				homeseekingMap.erase(it++);
			}
			else
			{
				++it;
			}
		}

	    if ((noFood && !homeseekingMap.size()) || IsEveryoneHomeAndFed())
		{
			KillUnfed();
			shouldDuplicate += awaitingMap.size();
			SetAllAwaitingFed(false); // sets all as unfed
			for (auto it = awaitingMap.begin(); it != awaitingMap.end(); )
			{
				hungryMap.insert({ it->first, it->second });
				graphicsSystem->SetColour(it->first, { 255, 255, 255, 255 });
				awaitingMap.erase(it++);
			}
		}
	}


	void SeekFood(int handle, glm::vec3 position, int adjustedDeltaTicks)
	{
		if (foodSystem->NoFood())
		{
			velocitySystem->SetVelocity(handle, 0.0f);
			return;
		}

		glm::vec3 nearestFood = foodSystem->FindNearestFoodToPoint(position);
		velocitySystem->SetVelocityAndDirection(handle, 0.05f, directionAToB(position, nearestFood));
	}

	glm::vec2 directionAToB(glm::vec3 A, glm::vec3 B)
	{
		return glm::normalize(glm::vec2{ B.x - A.x, B.y - A.y });
	}

	void SeekHome(int handle, glm::vec3 position, int adjusteddeltaTicks)
	{
		velocitySystem->SetVelocityAndDirection(handle, 0.05f, directionAToB({ 0.0f, 0.0f, 0.0f }, position));
	}

	void SetIsHome(int handle, bool set)
	{
		auto it = homeseekingMap.find(handle);
		if (it != homeseekingMap.end())
		{
			homeseekingMap[handle]->isHome = set;
		}
	}

	bool IsEveryoneHomeAndFed()
	{
		return (!hungryMap.size() && !homeseekingMap.size());
	}

	bool IsEveryoneHome()
	{
		return !homeseekingMap.size();
	}

	void KillUnfed()
	{
		for (auto it = hungryMap.begin(); it != hungryMap.end(); )
		{
			transformSystem->SetToBeDeleted(it->first);
			hungryMap.erase(it++);
		}
	}

	void SetFed(int handle, bool set)
	{
		auto it = hungryMap.find(handle);
		if (it != hungryMap.end())
		{
			it->second->isFed = set;
		}
	}

	void SetAllAwaitingFed(bool set)
	{
		for (auto it = awaitingMap.begin(); it != awaitingMap.end(); ++it)
		{
			it->second->isFed = set;
		}
	}

	void DeleteComponent(int handle)
	{
		hungryMap.erase(handle);
		homeseekingMap.erase(handle);
		awaitingMap.erase(handle);
	}
};
