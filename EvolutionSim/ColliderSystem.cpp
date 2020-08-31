#include <vector>
#include "GameData.h"
#include "Math.h"
#include "Grid.h"
#include "ColliderSystemSpecialization.h"
#include "PartitionSystemSpecialization.h"
#include "TransformSystemSpecialization.h"
#include "SurvivalSystemSpecialization.h"
#include "AggressionTraitSystemSpecialization.h"
#include "HungerTraitSystemSpecialization.h"
#include "StrengthTraitSystemSpecialization.h"

constexpr glm::vec2 EDGE_RIGHT_VEC2(0.01f, 0.0f);

using SurvivalState = SurvivalComponent::SurvivalState;

void animalOnAnimalCollision(Handle animalHandleA, Handle animalHandleB, float combatRateMultiplier, GameData& gameData)
{
	BeingEntity& animalA = gameData.beingManager.pool[animalHandleA];
	TransformComponent& transformA = animalA.transform;
	const glm::vec2& posA = Transform::System::getPos(transformA);
	const glm::vec2& dimsA = Transform::System::getDims(transformA);
	BeingEntity& animalB = gameData.beingManager.pool[animalHandleB];
	TransformComponent& transformB = animalB.transform;
	const glm::vec2& posB = Transform::System::getPos(transformB);
	const glm::vec2& dimsB = Transform::System::getDims(transformB);

	glm::vec2 distVec = { posA.x - posB.x, posA.y - posB.y };
	float distance = glm::length(distVec);
	float collisionDepth = ((dimsA.x + dimsA.y + dimsB.x + dimsB.y) / 4.0f) - distance;

	if (collisionDepth > 0)
	{
		SurvivalState survivalStateA = Survival::System::getSurvivalState(animalA.survival);
		SurvivalState survivalStateB = Survival::System::getSurvivalState(animalB.survival);

		if (!distance)
		{
			Transform::System::translate(
				transformA, 
				EDGE_RIGHT_VEC2);
		}
		else
		{
			glm::vec2 collisionDepthVec(glm::normalize(distVec) * collisionDepth * 0.5f);
			//StrengthTrait::System::resolveStrengthCollision(animalHandleA, animalHandleB, gameData, collisionDepthVec);
			Transform::System::translate(
				animalA.transform,
				collisionDepthVec);
			Transform::System::translate(
				animalB.transform,
				-collisionDepthVec);
		}

		if (survivalStateA == SurvivalState::SEARCHING && survivalStateB == SurvivalState::SEARCHING)
		{
			AggressionTrait::System::resolveAggressionTest(animalHandleA, animalHandleB, combatRateMultiplier, gameData);
		}
	}
}

void animalOnFoodCollision(Handle animalHandle, Handle foodHandle, float foodMultiplier, GameData& gameData)
{
	// collision check
	TransformComponent& transformA = gameData.beingManager.pool[animalHandle].transform;
	const glm::vec2& posA = Transform::System::getPos(transformA);
	const glm::vec2& dimsA = Transform::System::getDims(transformA);
	TransformComponent& transformB = gameData.foodManager.pool[foodHandle].transform;
	const glm::vec2& posB = Transform::System::getPos(transformB);
	const glm::vec2& dimsB = Transform::System::getDims(transformB);

	glm::vec2 distVec = { posA.x - posB.x, posA.y - posB.y };
	float distance = glm::length(distVec);
	float collisionDepth = ((dimsA.x + dimsA.y + dimsB.x + dimsB.y) / 4.0f) - distance;

	if (collisionDepth > 0)
	{
		HungerTrait::System::eatOnCollision(foodMultiplier, animalHandle, foodHandle, gameData);
	}
}

void ColliderSystem::update(const std::chrono::microseconds& dt, GameData& gameData)
{
	float timeRateModifier = static_cast<float>(dt.count());
	float physicsRateModifier = gameData.configurableSettings.physicsSpeed.get();
	float foodMultiplier = timeRateModifier * physicsRateModifier * gameData.configurableSettings.foodRateMultiplier.get();
	float combatMultiplier = timeRateModifier * physicsRateModifier * gameData.configurableSettings.combatRateMultiplier.get();

	Grid& animalGrid = Partition::System::getAnimalGrid(gameData.spatialPartition.partition);
	Grid& foodGrid = Partition::System::getFoodGrid(gameData.spatialPartition.partition);
	std::vector<BeingEntity> animalPool = gameData.beingManager.pool;
	std::vector<FoodEntity> foodPool = gameData.foodManager.pool;
	size_t cellCount = Math::getNOfCellsPerRow(gameData.configurableSettings.arenaDiameter.get(), CELL_SIZE);
	for (size_t i = 0; i < cellCount; i++) // map of possible x coords
	{
		for (size_t j = 0; j < cellCount; j++) // map of possible x coords
		{
			const Cell& cell = animalGrid[i][j];
			const size_t cellSize = cell.size();
			for (size_t k = 0; k < cellSize; k++) // map of y coord for each x coord
			{
				// same cell vs animal
				for (size_t l = 0; l < cellSize; l++)
				{
					if (l != k)
					{
						animalOnAnimalCollision(cell[k], cell[l], combatMultiplier, gameData);
					}
				}
				// same cell vs food
				const Cell& foodCell = foodGrid[i][j];
				for (size_t l = 0; l < foodCell.size(); l++)
				{
					animalOnFoodCollision(cell[k], foodCell[l], foodMultiplier, gameData);
				}

				if (i > 0 && j > 0) // nw cell
				{
					const Cell& nwAnimalCell = animalGrid[i - 1][j - 1];
					const size_t nwAnimalCellSize = nwAnimalCell.size();
					for (size_t l = 0; l < nwAnimalCellSize; l++)
					{
						animalOnAnimalCollision(cell[k], nwAnimalCell[l], combatMultiplier, gameData);
					}

					const Cell& nwFoodCell = foodGrid[i - 1][j - 1];
					const size_t nwFoodCellSize = nwFoodCell.size();
					for (size_t l = 0; l < nwFoodCellSize; l++)
					{
						animalOnFoodCollision(cell[k], nwFoodCell[l], foodMultiplier, gameData);
					}
				}

				if (i > 0) // n cell
				{
					const Cell& nAnimalCell = animalGrid[i - 1][j];
					const size_t nAnimalCellSize = nAnimalCell.size();
					for (size_t l = 0; l < nAnimalCellSize; l++)
					{
						animalOnAnimalCollision(cell[k], nAnimalCell[l], combatMultiplier, gameData);
					}

					const Cell& nFoodCell = foodGrid[i - 1][j];
					const size_t nFoodCellSize = nFoodCell.size();
					for (size_t l = 0; l < nFoodCellSize; l++)
					{
						animalOnFoodCollision(cell[k], nFoodCell[l], foodMultiplier, gameData);
					}
				}

				if (j > 0) // w cell
				{
					const Cell& wAnimalCell = animalGrid[i][j - 1];
					const size_t wAnimalCellSize = wAnimalCell.size();
					for (size_t l = 0; l < wAnimalCellSize; l++)
					{
						animalOnAnimalCollision(cell[k], wAnimalCell[l], combatMultiplier, gameData);
					}

					const Cell& wFoodCell = foodGrid[i][j - 1];
					const size_t wFoodCellSize = wFoodCell.size();
					for (size_t l = 0; l < wFoodCellSize; l++)
					{
						animalOnFoodCollision(cell[k], wFoodCell[l], foodMultiplier, gameData);
					}
				}
				
				if (i > 0 && j + 1 < cellCount) // ne cell
				{
					const Cell& neFoodCell = foodGrid[i - 1][j + 1];
					const size_t neFoodCellSize = neFoodCell.size();
					for (size_t l = 0; l < neFoodCellSize; l++)
					{
						animalOnFoodCollision(cell[k], neFoodCell[l], foodMultiplier, gameData);
					}
				}

				if (j + 1 < cellCount) // e cell
				{
					const Cell& eFoodCell = foodGrid[i][j + 1];
					const size_t eFoodCellSize = eFoodCell.size();
					for (size_t l = 0; l < eFoodCellSize; l++)
					{
						animalOnFoodCollision(cell[k], eFoodCell[l], foodMultiplier, gameData);
					}

				}

				if (j + 1 < cellCount && i + 1 < cellCount) // se cell
				{
					const Cell& seFoodCell = foodGrid[i + 1][j + 1];
					const size_t seFoodCellSize = seFoodCell.size();
					for (size_t l = 0; l < seFoodCellSize; l++)
					{
						animalOnFoodCollision(cell[k], seFoodCell[l], foodMultiplier, gameData);
					}
				}

				if (i + 1 < cellCount) // s cell
				{
					const Cell& sFoodCell = foodGrid[i + 1][j];
					const size_t sFoodCellSize = sFoodCell.size();
					for (size_t l = 0; l < sFoodCellSize; l++)
					{
						animalOnFoodCollision(cell[k], sFoodCell[l], foodMultiplier, gameData);
					}
				}
			}
		}
	}
}