#include <glm\geometric.hpp>
#include <glm\ext\vector_float2.hpp>

#include "BeingManager.h"
#include "SurvivalComponent.h"
#include "ColliderSystem.h"
#include "GeneSystem.h"
#include "PartitionSystem.h"
#include "SpriteSystem.h"
#include "SurvivalSystem.h"
#include "TextureSystem.h"
#include "TransformSystem.h"
#include "VelocitySystem.h"

using Handle = unsigned int;
using Cell = std::vector<Handle>;
using Row = std::vector<Cell>;
using Grid = std::vector<Row>;
using CollisionType = ColliderSystem::CollisionType;
using SurvivalState = SurvivalComponent::SurvivalState;
using Trait = GeneComponent::Trait;

void ColliderSystem::update(BeingManager& beings, unsigned int dt)
{
	float foodMultiplier = static_cast<float>(dt) * velocitySystem.getPhysicsSpeedVal() * FOOD_STRENGTH_MULTIPLIER;

	const Grid beingGrid = partitionSystem.partitionBeingsToGrid(beings);
	for (size_t i = 0; i < CELL_COUNT; i++) // map of possible x coords
	{
		for (size_t j = 0; j < CELL_COUNT; j++) // map of possible x coords
		{
			const Cell& cell = beingGrid[i][j];
			const size_t cellSize = cell.size();
			for (size_t k = 0; k < cellSize; k++) // map of y coord for each x coord
			{
				for (size_t l = 0; l < cellSize; l++)
				{
					if (l != k)
					{
						detectCollision(beings.pool[cell[k]], beings.pool[cell[l]], cell[k], cell[l], foodMultiplier);
					}
				}

				if (i > 0 && j > 0)
				{
					const Cell& nwCell = beingGrid[i - 1][j - 1];
					const size_t nwCellSize = nwCell.size();
					for (size_t l = 0; l < nwCellSize; l++)
					{
						detectCollision(beings.pool[cell[k]], beings.pool[nwCell[l]], cell[k], nwCell[l], foodMultiplier);
					}
				}

				if (i > 0)
				{
					const Cell& nCell = beingGrid[i - 1][j];
					const size_t nCellSize = nCell.size();
					for (size_t l = 0; l < nCellSize; l++)
					{
						detectCollision(beings.pool[cell[k]], beings.pool[nCell[l]], cell[k], nCell[l], foodMultiplier);
					}
				}

				if (j > 0)
				{
					const Cell& wCell = beingGrid[i][j - 1];
					const size_t wCellSize = wCell.size();
					for (size_t l = 0; l < wCellSize; l++)
					{
						detectCollision(beings.pool[cell[k]], beings.pool[wCell[l]], cell[k], wCell[l], foodMultiplier);
					}
				}
			}
		}
	}
}

std::set<Handle>& ColliderSystem::getToDelete(const BeingManager& beings)
{
	return toDelete;
}

constexpr glm::vec2 EDGE_RIGHT_VEC2(0.01f, 0.0f);

void ColliderSystem::detectCollision(Being& beingA, Being& beingB, const Handle handleA, const Handle handleB, const float foodMultiplier)
{
	// collision check
	TransformComponent& transformA = beingA.transform;
	const glm::vec2& posA = transformSystem.getPos(transformA);
	const glm::vec2& dimsA = transformSystem.getDims(transformA);
	TransformComponent& transformB = beingB.transform;
	const glm::vec2& posB = transformSystem.getPos(transformB);
	const glm::vec2& dimsB = transformSystem.getDims(transformB);

	glm::vec2 distVec = { posA.x - posB.x, posA.y - posB.y };
	float distance = glm::length(distVec);
	float collisionDepth = ((dimsA.x + dimsA.y + dimsB.x + dimsB.y) / 4.0f) - distance;

	if (collisionDepth > 0) 
	{
		// collision type
		bool AisAlive = survivalSystem.getIsAlive(beingA.survival);
		bool BisAlive = survivalSystem.getIsAlive(beingB.survival);
		CollisionType collisionType{ CollisionType::OTHER };
		if (AisAlive && BisAlive) //alive meat vs alive meat
		{
			collisionType = CollisionType::ALIVE_ANIMAL_VS_ALIVE_ANIMAL;
		}
		else if (AisAlive) //alive animal vs food
		{
			collisionType = CollisionType::ALIVE_ANIMAL_VS_OTHER;

		}
		else if (BisAlive) //food vs alive animal
		{
			collisionType = CollisionType::OTHER_VS_ALIVE_ANIMAL;
		}

		// collision consequence
		switch (collisionType)
		{
		case CollisionType::ALIVE_ANIMAL_VS_ALIVE_ANIMAL:
		{
			if (!distance)
			{
				transformSystem.translate(transformA, EDGE_RIGHT_VEC2);
			}
			else
			{
				glm::vec2 collisionDepthVec(glm::normalize(distVec) * collisionDepth * 0.5f);
				// TRANSFORM BASED ON STRENGTH
				if (survivalSystem.getSurvivalState(beingA.survival) == SurvivalState::AWAITING ||
					survivalSystem.getSurvivalState(beingB.survival) == SurvivalState::AWAITING)
				{
					transformSystem.translate(transformA, collisionDepthVec);
					transformSystem.translate(transformB, -collisionDepthVec);
				}

				float strengthA = geneSystem.getTrait(beingA.gene, Trait::STRENGTH);
				float strengthB = geneSystem.getTrait(beingB.gene, Trait::STRENGTH);

				if (strengthA - 0.05f > strengthB)
				{
					transformSystem.translate(transformB, -2.0f * collisionDepthVec);

				}
				else if (strengthB - 0.05f > strengthA)
				{
					transformSystem.translate(transformA, 2.0f * collisionDepthVec);
				}
				else
				{
					transformSystem.translate(transformA, collisionDepthVec);
					transformSystem.translate(transformB, -collisionDepthVec);
				}
			}
			break;
		}
		case CollisionType::ALIVE_ANIMAL_VS_OTHER:
		{
			eatOnCollision(foodMultiplier, beingA, beingB, handleB);
			break;
		}
		case CollisionType::OTHER_VS_ALIVE_ANIMAL:
		{
			eatOnCollision(foodMultiplier, beingB, beingA, handleA);
			break;
		}
		}
	}
}

void ColliderSystem::eatOnCollision(float foodMultiplier, Being& beingA, Being& beingB, const Handle handleB)
{
	if (survivalSystem.getSurvivalState(beingA.survival) == SurvivalState::SEARCHING)
	{
		// evaluate eat interaction through modification algorithm [could elaborate here]
		const float maxFullness = geneSystem.getTrait(beingA.gene, GeneComponent::Trait::HUNGER) * 1000.0f;
		const float maxFoodConsumable = maxFullness - survivalSystem.getBeingFullness(beingA.survival);
		const float maxFoodDepletable = survivalSystem.getBeingFullness(beingB.survival);
		const float dietType = geneSystem.getTrait(beingA.gene, Trait::DIET);
		float dietaryBonus = geneSystem.getBeingType(beingA.gene) == BeingType::PLANT ? pow((1.0f - dietType), 2) : pow(dietType, 2);
		float modify = std::min(foodMultiplier * dietaryBonus, maxFoodConsumable);
		modify = std::min(modify, maxFoodDepletable);
		survivalSystem.setFullness(beingA.survival, survivalSystem.getBeingFullness(beingA.survival) + modify);
		survivalSystem.setFullness(beingB.survival, survivalSystem.getBeingFullness(beingB.survival) - modify);

		if (survivalSystem.getBeingFullness(beingB.survival) <= 0)
		{
			toDelete.insert(handleB);
		}

		float fullness = survivalSystem.getBeingFullness(beingA.survival) / maxFullness;
		if (fullness >= 1.0f)
		{
			survivalSystem.setSurvivalState(beingA.survival, SurvivalState::RETURNING); // completely full
			spriteSystem.setTextureID(beingA.sprite, textureSystem.getTextureID(TextureSystem::Component::BEING_8));
		}
		else if (fullness >= 0.87f)
		{
			spriteSystem.setTextureID(beingA.sprite, textureSystem.getTextureID(TextureSystem::Component::BEING_7));
		}
		else if (fullness >= 0.74f)
		{
			spriteSystem.setTextureID(beingA.sprite, textureSystem.getTextureID(TextureSystem::Component::BEING_6));
		}
		else if (fullness >= 0.61f)
		{
			spriteSystem.setTextureID(beingA.sprite, textureSystem.getTextureID(TextureSystem::Component::BEING_5));
		}
		else if (fullness >= 0.48f)
		{
			spriteSystem.setTextureID(beingA.sprite, textureSystem.getTextureID(TextureSystem::Component::BEING_4));
		}
		else if (fullness >= 0.35f)
		{
			spriteSystem.setTextureID(beingA.sprite, textureSystem.getTextureID(TextureSystem::Component::BEING_3));
		}
		else if (fullness >= 0.22f)
		{
			spriteSystem.setTextureID(beingA.sprite, textureSystem.getTextureID(TextureSystem::Component::BEING_2));
		}
		else if (fullness >= 0.10f)
		{
			spriteSystem.setTextureID(beingA.sprite, textureSystem.getTextureID(TextureSystem::Component::BEING_1));
		}
	}
}
