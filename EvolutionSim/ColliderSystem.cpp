#include "ColliderSystem.h"
#include "VelocitySystem.h"
#include "BeingManager.h"
#include "PartitionSystem.h"
#include "SurvivalSystem.h"
#include "GeneSystem.h"

#include <glm\geometric.hpp>

void ColliderSystem::process(BeingManager& beings, long long dt)
{
	dt *= velocitySystem.getPhysicsSpeedVal();

	const Grid beingGrid = partitionSystem.processBeingGrid(beings);
	for (uint32_t i = 0; i < CELL_COUNT; i++) // map of possible x coords
	{
		for (uint32_t j = 0; j < CELL_COUNT; j++) // map of possible x coords
		{
			const Cell& cell = beingGrid[i][j];
			uint32_t cellSize = cell.size();
			for (uint32_t k = 0; k < cellSize; k++) // map of y coord for each x coord
			{
				for (uint32_t l = 0; l < cellSize; l++)
				{
					if (l != k)
					{
						detectCollision(beings, cell[k], cell[l], dt);
					}
				}

				if (i > 0 && j > 0)
				{
					const Cell& NWcell = beingGrid[i - 1][j - 1];
					for (uint32_t l = 0; l < NWcell.size(); l++)
					{
						detectCollision(beings, cell[k], NWcell[l], dt);
					}
				}

				if (i > 0)
				{
					const Cell& Ncell = beingGrid[i - 1][j];
					for (uint32_t l = 0; l < Ncell.size(); l++)
					{
						detectCollision(beings, cell[k], Ncell[l], dt);
					}
				}

				if (j > 0)
				{
					const Cell& Wcell = beingGrid[i][j - 1];
					for (uint32_t l = 0; l < Wcell.size(); l++)
					{
						detectCollision(beings, cell[k], Wcell[l], dt);
					}
				}
			}
		}
	}
}

void ColliderSystem::detectCollision(BeingManager& beings, const uint32_t handleA, const uint32_t handleB, int dt)
{
	dt *= foodStrengthMultiplier;
	TransformComponent& transformA = beings.getTransformComponent(handleA);
	const glm::vec2& posA = transformSystem.getPos(transformA);
	const glm::vec2& dimsA = transformSystem.getDims(transformA);
	TransformComponent& transformB = beings.getTransformComponent(handleB);
	const glm::vec2& posB = transformSystem.getPos(transformB);
	const glm::vec2& dimsB = transformSystem.getDims(transformB);

	glm::vec2 distVec = { posA.x - posB.x, posA.y - posB.y };
	float distance = glm::length(distVec);
	float minDistance = (dimsA.x + dimsA.y + dimsB.x + dimsB.y) / 4.0f;
	float collisionDepth = minDistance - distance;
	{
		if (collisionDepth > 0) // Collision
		{
			CollisionType collisionType = detectCollisionType(beings, handleA, handleB);

			switch (collisionType)
			{
			case CollisionType::ALIVE_ANIMAL_VS_ALIVE_ANIMAL:
			{
				if (!distance)
				{
					transformSystem.translate(transformA, glm::vec2(0.01f, 0.0f));
					transformSystem.translate(transformB, glm::vec2(-0.01f, 0.0f));
				}
				else
				{
					glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth * 0.5f;
					transformSystem.translate(transformA, collisionDepthVec);
					transformSystem.translate(transformB, -collisionDepthVec);
				}
				break;
			}
			case CollisionType::ALIVE_ANIMAL_VS_OTHER:
			{
				survivalSystem.eating(dt, beings, handleA, handleB);
				break;
			}
			case CollisionType::OTHER_VS_ALIVE_ANIMAL:
			{
				survivalSystem.eating(dt, beings, handleB, handleA);
				break;
			}
			}
		}
	}
}

CollisionType ColliderSystem::detectCollisionType(BeingManager& beings, const uint32_t handleA, const uint32_t handleB)
{
	const GeneComponent& geneA = beings.getGeneComponent(handleA);
	const GeneComponent& geneB = beings.getGeneComponent(handleB);
	const SurvivalComponent& survivalA = beings.getSurvivalComponent(handleA);
	const SurvivalComponent& survivalB = beings.getSurvivalComponent(handleB);

	//NTS: getBeingFoodType returns a flag, plant = 2, animal = 4
	int aState = static_cast<int>(survivalSystem.getIsAlive(survivalA)) | static_cast<int>(geneSystem.getBeingType(geneA));
	int bState = static_cast<int>(survivalSystem.getIsAlive(survivalB)) | static_cast<int>(geneSystem.getBeingType(geneB));

	if ((aState & bState) == ((static_cast<int>(GeneComponent::BeingType::ANIMAL)) | true)) //alive meat vs alive meat
	{
		return CollisionType::ALIVE_ANIMAL_VS_ALIVE_ANIMAL;
	}
	else if (aState == ((static_cast<int>(GeneComponent::BeingType::ANIMAL)) | true)) //alive animal vs food
	{
		return CollisionType::ALIVE_ANIMAL_VS_OTHER;
	}
	else if (bState == ((static_cast<int>(GeneComponent::BeingType::ANIMAL)) | true)) //food vs alive animal
	{
		return CollisionType::OTHER_VS_ALIVE_ANIMAL;
	}
	else
	{
		return CollisionType::OTHER;
	}
}


