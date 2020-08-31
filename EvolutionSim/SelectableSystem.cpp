#include "GameData.h"
#include "SelectableSystemSpecialization.h"
#include "TransformSystemSpecialization.h"
#include "Handle.h"

void SelectableSystem::processClick(const glm::vec2& mouseCoords, GameData& gameData) const
{
	float selectionProximity = FLT_MAX;
	Handle tempFoodSelected{ UINT_MAX };
	Handle tempBeingSelected{ UINT_MAX };

	size_t foodSize = gameData.foodManager.pool.size();
	Manager<FoodEntity> foodManager = gameData.foodManager;
	for (Handle foodHandle = 0; foodHandle < foodSize; ++foodHandle)
	{
		const glm::vec2& foodPos = Transform::System::getPos(foodManager.pool[foodHandle].transform);
		const glm::vec2& foodDims = Transform::System::getDims(foodManager.pool[foodHandle].transform);
		const float xRadius = foodDims.x / 2.0f;
		const float yRadius = foodDims.y / 2.0f;

		if (mouseCoords.x < foodPos.x + xRadius
			&& mouseCoords.x > foodPos.x - xRadius
			&& mouseCoords.y < foodPos.y + yRadius
			&& mouseCoords.y > foodPos.y - yRadius)
		{
			const float xDist = abs(mouseCoords.x - foodPos.x);
			const float yDist = abs(mouseCoords.y - foodPos.y);
			const float dist = xDist * xDist + yDist * yDist;
			if (dist < selectionProximity)
			{
				selectionProximity = dist;
				tempFoodSelected = foodHandle;
			}
		}
	}

	size_t beingsSize = gameData.beingManager.pool.size();
	Manager<BeingEntity> beingManager = gameData.beingManager;
	for (Handle beingHandle = 0; beingHandle < beingsSize; ++beingHandle)
	{
		const glm::vec2& beingPos = Transform::System::getPos(beingManager.pool[beingHandle].transform);
		const glm::vec2& beingDims = Transform::System::getDims(beingManager.pool[beingHandle].transform);
		const float xRadius = beingDims.x / 2.0f;
		const float yRadius = beingDims.y / 2.0f;

		if (mouseCoords.x < beingPos.x + xRadius
			&& mouseCoords.x > beingPos.x - xRadius
			&& mouseCoords.y < beingPos.y + yRadius
			&& mouseCoords.y > beingPos.y - yRadius)
		{
			const float xDist = abs(mouseCoords.x - beingPos.x);
			const float yDist = abs(mouseCoords.y - beingPos.y);
			const float dist = xDist * xDist + yDist * yDist;
			if (dist < selectionProximity)
			{
				selectionProximity = dist;
				tempBeingSelected = beingHandle;
				tempFoodSelected = UINT_MAX;
			}
		}
	}

	//For now we'll allow selection of both being and food - we can later choose how it's dealt with

	if (tempBeingSelected < UINT_MAX)
	{
		Selectable::System::setSelectedAnimalHandle(gameData.selectedTracker.component, tempBeingSelected);
	}
	else if (tempFoodSelected < UINT_MAX)
	{
		Selectable::System::setFoodHandle(gameData.selectedTracker.component, tempFoodSelected);
		Selectable::System::setSelectedAnimalHandle(gameData.selectedTracker.component, UINT_MAX);
	}
}

void SelectableSystem::update(GameData& gameData) const
{
	SelectedComponent& selected = gameData.selectedTracker.component;
	if (Selectable::System::getSelectedAnimalHandle(selected) < gameData.beingManager.getSize()) // is valid handle
	{
		Transform::System::setPos(gameData.selectionBox.transform, Transform::System::getPos(gameData.beingManager.pool[Selectable::System::getSelectedAnimalHandle(selected)].transform));
	}
	else if (Selectable::System::getFoodHandle(selected) < gameData.foodManager.getSize())
	{
		Transform::System::setPos(gameData.selectionBox.transform, Transform::System::getPos(gameData.foodManager.pool[Selectable::System::getFoodHandle(selected)].transform));
	}
	else
	{
		Transform::System::setPos(gameData.selectionBox.transform, glm::vec2(FLT_MAX));
	}
}

void SelectableSystem::clearSelectedHandle(SelectedComponent& component) const
{
	Selectable::System::setSelectedAnimalHandle(component, UINT_MAX);
	Selectable::System::setFoodHandle(component, UINT_MAX);
}