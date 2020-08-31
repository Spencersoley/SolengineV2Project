#include "PartitionSystemSpecialization.h"
#include "TransformSystemSpecialization.h"
#include "GameData.h"
#include "Math.h"
#include <glm\geometric.hpp>

void SpatialPartitionSystem::update(GameData& gameData) const
{
	Grid& animalGrid = Partition::System::getAnimalGrid(gameData.spatialPartition.partition);
	size_t cellCount = Math::getNOfCellsPerRow(gameData.configurableSettings.arenaDiameter.get(), CELL_SIZE);
	for (size_t i = 0; i < cellCount; ++i)
	{
		for (size_t j = 0; j < cellCount; ++j)
		{
			animalGrid[i][j].clear();
		}
	}

	const float bounded = gameData.configurableSettings.bounded.get();
	auto arenaDiameter = gameData.configurableSettings.arenaDiameter.get();
	std::vector<BeingEntity>& animalPool = gameData.beingManager.pool;
	for (Handle animalHandle = 0; animalHandle < animalPool.size(); animalHandle++)
	{
		glm::vec2 pos{ Transform::System::getPos(animalPool[animalHandle].transform) };
		if (bounded)
		{
			float distanceOutOfBounds = glm::distance(pos, { 0, 0 }) - (arenaDiameter * 1.1f / 2.0f);
			if (distanceOutOfBounds > 0)
			{
				glm::vec2 normalizedCorrectionVec = glm::normalize(-pos);
				Transform::System::translate(animalPool[animalHandle].transform, normalizedCorrectionVec * distanceOutOfBounds);
				pos = Transform::System::getPos(animalPool[animalHandle].transform);
			}
		}

		int x = static_cast<int>((pos.x / CELL_SIZE) + (cellCount / 2));
		x = std::min(std::max(0, x), static_cast<int>(cellCount - 1));
		int y = static_cast<int>((pos.y / CELL_SIZE) + (cellCount / 2));
		y = std::min(std::max(0, y), static_cast<int>(cellCount - 1));

		animalGrid[x][y].push_back(animalHandle);
	}

	Grid& foodGrid = Partition::System::getFoodGrid(gameData.spatialPartition.partition);
	for (size_t i = 0; i < cellCount; ++i)
	{
		for (size_t j = 0; j < cellCount; ++j)
		{
			foodGrid[i][j].clear();
		}
	}

	std::vector<FoodEntity>& foodPool = gameData.foodManager.pool;
	for (Handle foodHandle = 0; foodHandle < foodPool.size(); foodHandle++)
	{
		glm::vec2 pos{ Transform::System::getPos(foodPool[foodHandle].transform) };
		if (bounded)
		{
			float distanceOutOfBounds = glm::distance(pos, { 0, 0 }) - (arenaDiameter / 2.0f);
			if (distanceOutOfBounds > 0)
			{
				glm::vec2 normalizedCorrectionVec = glm::normalize(-pos);
				Transform::System::translate(foodPool[foodHandle].transform, normalizedCorrectionVec * distanceOutOfBounds);
				pos = Transform::System::getPos(foodPool[foodHandle].transform);
			}
		}

		int x = static_cast<int>((pos.x / CELL_SIZE) + (cellCount / 2));
		x = std::min(std::max(0, x), static_cast<int>(cellCount - 1));
		int y = static_cast<int>((pos.y / CELL_SIZE) + (cellCount / 2));
		y = std::min(std::max(0, y), static_cast<int>(cellCount - 1));

		foodGrid[x][y].push_back(foodHandle);
	}
}

void SpatialPartitionSystem::setupGrid(PartitionComponent& component, GameData& gameData) const
{
	size_t cellCount = Math::getNOfCellsPerRow(gameData.configurableSettings.arenaDiameter.get(), CELL_SIZE);
	Partition::System::setAnimalGrid(component, Grid(cellCount, Row(cellCount, Cell{})));
	Partition::System::setFoodGrid(component, Grid(cellCount, Row(cellCount, Cell{})));
}