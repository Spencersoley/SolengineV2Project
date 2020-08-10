#include <vector>
#include "BeingManager.h"
#include "GenerationManager.h"

#include "ArenaSize.h"
#include "GeneEnum.h"

#include "ColliderSystemImplementation.h"
#include "TextureLoaderSystemImplementation.h"
#include "VelocitySystemImplementation.h"
#include "PartitionSystemImplementation.h"
#include "TransformSystemImplementation.h"
#include "GeneSystemImplementation.h"
#include "GenerationSystemImplementation.h"
#include "SpriteSystemImplementation.h"
#include "SurvivalSystemImplementation.h"

const float FOOD_STRENGTH_MULTIPLIER{ 0.00001f };
constexpr glm::vec2 EDGE_RIGHT_VEC2(0.01f, 0.0f);

using SurvivalState = SurvivalComponent::SurvivalState;
using Trait = Gene::Trait;
using BeingType = Gene::BeingType;
using Handle = unsigned int;
using Cell = std::vector<Handle>;
using Row = std::vector<Cell>;
using Grid = std::vector<Row>;

enum class CollisionType
{
	ALIVE_ANIMAL_VS_ALIVE_ANIMAL = 1 << 1,
	ALIVE_ANIMAL_VS_OTHER = 1 << 2,
	OTHER_VS_ALIVE_ANIMAL = 1 << 3,
	OTHER = 1 << 4
};

void ColliderSystem::update(BeingManager& beings, PartitionComponent& partition, GenerationManager& generationManager, std::set<Handle>& handlesToDelete, unsigned int dt, TextureLibrary& textureLibrary)
{
	float foodMultiplier = static_cast<float>(dt) * Generation::System::getPhysicsSpeed(generationManager.component) * FOOD_STRENGTH_MULTIPLIER;

	const Grid& beingGrid = Partition::System::getGrid(partition);
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
						detectCollision(beings.pool[cell[k]], beings.pool[cell[l]], cell[k], cell[l], foodMultiplier, handlesToDelete, textureLibrary);
					}
				}

				if (i > 0 && j > 0)
				{
					const Cell& nwCell = beingGrid[i - 1][j - 1];
					const size_t nwCellSize = nwCell.size();
					for (size_t l = 0; l < nwCellSize; l++)
					{
						detectCollision(beings.pool[cell[k]], beings.pool[nwCell[l]], cell[k], nwCell[l], foodMultiplier, handlesToDelete, textureLibrary);
					}
				}

				if (i > 0)
				{
					const Cell& nCell = beingGrid[i - 1][j];
					const size_t nCellSize = nCell.size();
					for (size_t l = 0; l < nCellSize; l++)
					{
						detectCollision(beings.pool[cell[k]], beings.pool[nCell[l]], cell[k], nCell[l], foodMultiplier, handlesToDelete, textureLibrary);
					}
				}

				if (j > 0)
				{
					const Cell& wCell = beingGrid[i][j - 1];
					const size_t wCellSize = wCell.size();
					for (size_t l = 0; l < wCellSize; l++)
					{
						detectCollision(beings.pool[cell[k]], beings.pool[wCell[l]], cell[k], wCell[l], foodMultiplier, handlesToDelete, textureLibrary);
					}
				}
			}
		}
	}
}

void ColliderSystem::detectCollision(Being& beingA, Being& beingB, const Handle handleA, const Handle handleB, const float foodMultiplier, std::set<Handle>& handlesToDelete, TextureLibrary& textureLibrary)
{
	// collision check
	TransformComponent& transformA = beingA.transform;
	const glm::vec2& posA = Transform::System::getPos(transformA);
	const glm::vec2& dimsA = Transform::System::getDims(transformA);
	TransformComponent& transformB = beingB.transform;
	const glm::vec2& posB = Transform::System::getPos(transformB);
	const glm::vec2& dimsB = Transform::System::getDims(transformB);

	glm::vec2 distVec = { posA.x - posB.x, posA.y - posB.y };
	float distance = glm::length(distVec);
	float collisionDepth = ((dimsA.x + dimsA.y + dimsB.x + dimsB.y) / 4.0f) - distance;

	if (collisionDepth > 0)
	{
		// collision type
		bool AisAlive = Survival::System::getIsAlive(beingA.survival);
		bool BisAlive = Survival::System::getIsAlive(beingB.survival);
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
				Transform::System::translate(transformA, EDGE_RIGHT_VEC2);
			}
			else
			{
				glm::vec2 collisionDepthVec(glm::normalize(distVec) * collisionDepth * 0.5f);
				// TRANSFORM BASED ON STRENGTH
				if (Survival::System::getSurvivalState(beingA.survival) == SurvivalState::AWAITING ||
					Survival::System::getSurvivalState(beingB.survival) == SurvivalState::AWAITING)
				{
					Transform::System::translate(transformA, collisionDepthVec);
					Transform::System::translate(transformB, -collisionDepthVec);
				}

				float strengthA = Gene::System::getTrait(beingA.gene, Trait::STRENGTH);
				float strengthB = Gene::System::getTrait(beingB.gene, Trait::STRENGTH);

				if (strengthA - 0.05f > strengthB)
				{
					Transform::System::translate(transformB, -2.0f * collisionDepthVec);

				}
				else if (strengthB - 0.05f > strengthA)
				{
					Transform::System::translate(transformA, 2.0f * collisionDepthVec);
				}
				else
				{
					Transform::System::translate(transformA, collisionDepthVec);
					Transform::System::translate(transformB, -collisionDepthVec);
				}
			}
			break;
		}
		case CollisionType::ALIVE_ANIMAL_VS_OTHER:
		{
			eatOnCollision(foodMultiplier, beingA, beingB, handleB, handlesToDelete, textureLibrary);
			break;
		}
		case CollisionType::OTHER_VS_ALIVE_ANIMAL:
		{
			eatOnCollision(foodMultiplier, beingB, beingA, handleA, handlesToDelete, textureLibrary);
			break;
		}
		}
	}
}

void ColliderSystem::eatOnCollision(float foodMultiplier, Being& beingA, Being& beingB, const Handle handleB, std::set<Handle>& handlesToDelete, TextureLibrary& textureLibrary)
{
	if (Survival::System::getSurvivalState(beingA.survival) == SurvivalState::SEARCHING)
	{
		// evaluate eat interaction through modification algorithm [could elaborate here]
		const float maxFullness = Gene::System::getTrait(beingA.gene, Gene::Trait::HUNGER) * 1000.0f;
		const float maxFoodConsumable = maxFullness - Survival::System::getFullness(beingA.survival);
		const float maxFoodDepletable = Survival::System::getFullness(beingB.survival);
		const float dietType = Gene::System::getTrait(beingA.gene, Trait::DIET);
		float dietaryBonus = Gene::System::getBeingType(beingA.gene) == BeingType::PLANT ? pow((1.0f - dietType), 2) : pow(dietType, 2);
		float modify = std::min(foodMultiplier * dietaryBonus, maxFoodConsumable);
		modify = std::min(modify, maxFoodDepletable);
		Survival::System::setFullness(beingA.survival, Survival::System::getFullness(beingA.survival) + modify);
		Survival::System::setFullness(beingB.survival, Survival::System::getFullness(beingB.survival) - modify);

		if (Survival::System::getFullness(beingB.survival) <= 0)
		{
			handlesToDelete.insert(handleB);
		}

		float fullness = Survival::System::getFullness(beingA.survival) / maxFullness;
		if (fullness >= 1.0f)
		{
			Survival::System::setSurvivalState(beingA.survival, SurvivalState::RETURNING); // completely full
			Sprite::System::setTextureID(beingA.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::BEING_8, textureLibrary));
		}
		else if (fullness >= 0.87f)
		{
			Sprite::System::setTextureID(beingA.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::BEING_7, textureLibrary));
		}
		else if (fullness >= 0.74f)
		{
			Sprite::System::setTextureID(beingA.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::BEING_6, textureLibrary));
		}
		else if (fullness >= 0.61f)
		{
			Sprite::System::setTextureID(beingA.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::BEING_5, textureLibrary));
		}
		else if (fullness >= 0.48f)
		{
			Sprite::System::setTextureID(beingA.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::BEING_4, textureLibrary));
		}
		else if (fullness >= 0.35f)
		{
			Sprite::System::setTextureID(beingA.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::BEING_3, textureLibrary));
		}
		else if (fullness >= 0.22f)
		{
			Sprite::System::setTextureID(beingA.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::BEING_2, textureLibrary));
		}
		else if (fullness >= 0.10f)
		{
			Sprite::System::setTextureID(beingA.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::BEING_1, textureLibrary));
		}
	}
}
