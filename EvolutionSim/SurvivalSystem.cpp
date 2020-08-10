#include "BeingManager.h"
#include "GenerationManager.h"
#include "TextureLibrary.h"
#include "DefaultColours.h"
#include "ArenaSize.h"

#include "TextureLoaderSystemImplementation.h"
#include "SpriteSystemImplementation.h"
#include "TransformSystemImplementation.h"
#include "GeneSystemImplementation.h"
#include "GenerationSystemImplementation.h"
#include "VelocitySystemImplementation.h"

#include "SurvivalSystemImplementation.h"
#include <glm\geometric.hpp>

using SurvivalState = SurvivalComponent::SurvivalState;
constexpr float DEPLETION_MODIFIER = 0.0000005f;

void SurvivalSystem::update(BeingManager& beings, uint32_t dt, GenerationManager& generationManager, TextureLibrary& textureLibrary)
{
	int waveState{ static_cast<int>(WaveState::NOTHING_ACTIVE) };
	Generation::System::setWaveOver(generationManager.component, false);

	// Get handles of all beings which are food
	if (Generation::System::getFoodHandlesNeedUpdate(generationManager.component))
	{
		std::vector<Handle>& foodHandles = Generation::System::getFoodHandles(generationManager.component);
		foodHandles.clear();
		size_t poolSize = beings.pool.size();
		for (Handle beingHandle = 0; beingHandle < poolSize; beingHandle++)
		{
			if (!Survival::System::getIsAlive(beings.pool[beingHandle].survival))
			{
				foodHandles.push_back(beingHandle);
			}
		}

		Generation::System::setFoodHandlesNeedUpdate(generationManager.component, false);
	}

	static unsigned int frameCount = 0;
	if (++frameCount % 20 != 0) return;
	if (frameCount > 1000) frameCount = 0;

	float depletion = DEPLETION_MODIFIER * static_cast<float>(dt) * Generation::System::getPhysicsSpeed(generationManager.component);

	const auto processSurvivalBehaviour = [this, &waveState, depletion, &beings, &generationManager, &textureLibrary](Being& being)
	{
		if (!Survival::System::getIsAlive(being.survival))
		{
			waveState = waveState | static_cast<int>(WaveState::FOOD_ACTIVE); //there IS food
			wait(being);
		}
		else if (Survival::System::getSurvivalState(being.survival) == SurvivalState::AWAITING)
		{
			wait(being);
		}
		else if (Survival::System::getSurvivalState(being.survival) == SurvivalState::SEARCHING)
		{
			if (depleteEnergyAndReturnIsZero(being.survival, depletion))
			{
				killAnimal(being, textureLibrary);
			}
			else
			{
				// SEEK FOOD
				const glm::vec2& beingPosition = Transform::System::getPos(being.transform);
				const glm::vec2 nearestFoodPosition = findNearestFoodToPoint(beingPosition, beings, Generation::System::getFoodHandles(generationManager.component));
				if (nearestFoodPosition == beingPosition) // if no food found
				{
					Velocity::System::setVelocity(being.velocity, 0.0f);
				}
				else
				{
					const glm::vec2& directionToFood = glm::normalize(glm::vec2(nearestFoodPosition.x - beingPosition.x, nearestFoodPosition.y - beingPosition.y));
					Velocity::System::setDirection(being.velocity, directionToFood);
					Velocity::System::setVelocity(being.velocity, Gene::System::getTrait(being.gene, Trait::SPEED));
				}
				waveState = waveState | static_cast<int>(WaveState::SEARCHERS_ACTIVE); //there IS searchers
			}
		}
		else if (Survival::System::getSurvivalState(being.survival) == SurvivalState::RETURNING)
		{
			if (depleteEnergyAndReturnIsZero(being.survival, depletion))
			{
				killAnimal(being, textureLibrary);
			}
			else
			{
				// SEEK HOME 
				const glm::vec2& pos = Transform::System::getPos(being.transform);
				if (glm::length(pos) > ARENA_SIZE / 2.0f)
				{
					Survival::System::setSurvivalState(being.survival, SurvivalState::AWAITING);
				}
				else
				{
					Velocity::System::setVelocity(being.velocity, Gene::System::getTrait(being.gene, Trait::SPEED));
					//direction is normalised position, since we're just retreating from the centre.
					Velocity::System::setDirection(being.velocity, glm::normalize(pos));
				}
				waveState = waveState | static_cast<int>(WaveState::RETURNERS_ACTIVE); //there IS returners
			}
		}
	};

	for_each(begin(beings.pool), end(beings.pool), processSurvivalBehaviour);

	if (!(waveState & static_cast<int>(WaveState::FOOD_ACTIVE))) // if NO food
	{
		const auto killIfStillSearching = [this, &textureLibrary](Being& being)
		{
			if (Survival::System::getSurvivalState(being.survival) == SurvivalState::SEARCHING)
			{
				killAnimal(being, textureLibrary);
			}
		};

		for_each(begin(beings.pool), end(beings.pool), killIfStillSearching);
	}

	if (!(waveState & static_cast<int>(WaveState::RETURNERS_ACTIVE)) && !(waveState & static_cast<int>(WaveState::SEARCHERS_ACTIVE))) // if NO returners and NO searchers
	{
		Generation::System::setWaveOver(generationManager.component, true);
	}
}

glm::vec2 SurvivalSystem::findNearestFoodToPoint(const glm::vec2& point, const BeingManager& beings, const std::vector<Handle>& foodHandles) const
{
	glm::vec2 nearest(-FLT_MAX);
	float defaultClosest = glm::distance(nearest, point);
	float closestDist = defaultClosest;

	const auto compareToClosest = [&nearest, &closestDist, &beings, &point](Handle handle)
	{
		const glm::vec2& foodPos = Transform::System::getPos(beings.pool[handle].transform);
		const float dist = glm::distance(foodPos, point);
		if (dist < closestDist)
		{
			nearest = foodPos;
			closestDist = dist;
		}
	};

	std::for_each(cbegin(foodHandles), cend(foodHandles), compareToClosest);

	return (defaultClosest == closestDist) ? point : nearest;
}

bool SurvivalSystem::depleteEnergyAndReturnIsZero(SurvivalComponent& component, float depletion) const
{
	float energy = std::max(Survival::System::getEnergy(component) - depletion, 0.0f);

	Survival::System::setEnergy(component, energy);

	return energy == 0.0f;
}

void SurvivalSystem::wait(Being& being) const
{
	Velocity::System::setVelocity(being.velocity, 0);
}

void SurvivalSystem::killAnimal(Being& being, TextureLibrary& textureLibrary)
{
	Sprite::System::setColour(being.sprite, DEAD_ANIMAL_COLOUR);
	Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::MEAT, textureLibrary));
	Survival::System::setSurvivalState(being.survival, SurvivalState::AWAITING);
	Survival::System::setIsAlive(being.survival, false);
}