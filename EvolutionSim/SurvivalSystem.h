#pragma once
#include <algorithm>
#include "TransformSystem.h"
#include "FoodSystem.h"
#include "VelocitySystem.h"
#include "DuplicationData.h"
#include "StatSheet.h"
#include "SpriteSystem.h"
#include "TextSystem.h"


class SurvivalComponent
{
private:
	friend class SurvivalSystem;
	SurvivalComponent(std::shared_ptr<TransformComponent> _transform, StatSheet _statSheet)
		:
		transform(_transform),
		isFed(0),
		isHome(0),
		statSheet(_statSheet)
	{}

	StatSheet statSheet;
	std::shared_ptr<TransformComponent> transform;
	bool isHome;   // collider system knows of survival system, thus it can set isHome on collision with home
	bool isFed;    // Collider system knows of survival system, thus it can set isFed on collision with food
};

class SurvivalSystem
{
	TransformSystem* transformSystem;
	FoodSystem* foodSystem;
	VelocitySystem* velocitySystem;
	SpriteSystem* spriteSystem;
	TextSystem* textSystem;

	std::map<int, SurvivalComponent*> hungryMap{};
	std::map<int, SurvivalComponent*> homeseekingMap{};
	std::map<int, SurvivalComponent*> awaitingMap{};

	int waveCount = 0;
	int waveTextHandle = -1;

public:
	SurvivalSystem(
		TransformSystem* _transformSystem,
		VelocitySystem* _velocitySystem,
		FoodSystem* _foodSystem,
		SpriteSystem* _spriteSystem,
		TextSystem* _textSystem
	)
		:
		transformSystem(_transformSystem),
		foodSystem(_foodSystem),
		velocitySystem(_velocitySystem),
		spriteSystem(_spriteSystem),
		textSystem(_textSystem)
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

	std::vector<DuplicationData> DuplicationDataVec{};

	void AddComponent(int handle, StatSheet statSheet)
	{
		SurvivalComponent* surv = new SurvivalComponent(transformSystem->GetLast(), statSheet);
		hungryMap.try_emplace(handle, surv);
		spriteSystem->SetColour(handle, { 255, 255, 255, 255 });
	}

	void Process(int adjustedDeltaTicks)
	{
		bool noFood = foodSystem->NoFood();

		for (auto it = hungryMap.begin(); it != hungryMap.end(); )
		{
			if (it->second->isFed) //set in collider
			{
				homeseekingMap.insert({ it->first, it->second });
				SeekHome(it->first, transformSystem->GetPos(it->second->transform.get()), adjustedDeltaTicks, it->second->statSheet.Speed); //sets off for home
				SetIsHome(it->first, false);
				//graphicsSystem->SetColour(it->first, { 0, 255, 0, 255 });
				hungryMap.erase(it++);
			}
			else
			{
				SeekFood(it->first, transformSystem->GetPos(it->second->transform.get()), adjustedDeltaTicks, it->second->statSheet.Speed); //repeatedly seeks nearest food
				if (noFood) spriteSystem->SetColour(it->first, { 255, 0, 0, 255 });
				++it;
			}
		}

		for (auto it = homeseekingMap.begin(); it != homeseekingMap.end(); )
		{
			if (it->second->isHome) //set in collider
			{
				awaitingMap.insert({ it->first, it->second });
				velocitySystem->SetVelocity(it->first, 0.0f);
				//graphicsSystem->SetColour(it->first, { 0, 255, 255, 255 });
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

			textSystem->UpdateText(waveTextHandle, std::to_string(++waveCount));

			SetAllAwaitingFed(false); // sets all as unfed
			for (auto it = awaitingMap.begin(); it != awaitingMap.end(); )
			{
				hungryMap.insert({ it->first, it->second });
				StatSheet inherited = it->second->statSheet;
				float r = -0.02f + (float)(rand()) / (float)(RAND_MAX / 0.1f);
				inherited.Speed += r;
				if (inherited.Speed < 0.02f) inherited.Speed = 0.02f;
				//else if (inherited.Speed > 1.0f) inherited.Speed = 1.0f;
				DuplicationDataVec.push_back(DuplicationData(transformSystem->GetPos(it->second->transform.get()), inherited)); //add to duplication data
				awaitingMap.erase(it++);
			}
		}
	}

	void SeekFood(int handle, glm::vec3 position, int adjustedDeltaTicks, float speed) const
	{
		if (foodSystem->NoFood())
		{
			velocitySystem->SetVelocity(handle, 0.0f);
			return;
		}

		glm::vec3 nearestFood = foodSystem->FindNearestFoodToPoint(position);
		velocitySystem->SetVelocityAndDirection(handle, speed, directionAToB(position, nearestFood));
	}

	glm::vec2 directionAToB(glm::vec3 A, glm::vec3 B) const
	{
		return glm::normalize(glm::vec2{ B.x - A.x, B.y - A.y });
	}

	void SeekHome(int handle, glm::vec3 position, int adjusteddeltaTicks, float speed) const
	{
		velocitySystem->SetVelocityAndDirection(handle, speed, directionAToB({ 0.0f, 0.0f, 0.0f }, position));
	}

	void SetIsHome(int handle, bool set) 
	{
		auto it = homeseekingMap.find(handle);
		if (it != homeseekingMap.end())
		{
			homeseekingMap[handle]->isHome = set;
		}
	}

	bool IsEveryoneHomeAndFed() const
	{
		return (!hungryMap.size() && !homeseekingMap.size());
	}

	bool IsEveryoneHome() const
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

	void SetFed(int handle, bool set) const
	{
		auto it = hungryMap.find(handle);
		if (it != hungryMap.end())
		{
			it->second->isFed = set;
		}
	}

	void SetAllAwaitingFed(bool set) const
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

	void SetWaveTextHandle(int handle) { waveTextHandle = handle; }
};
