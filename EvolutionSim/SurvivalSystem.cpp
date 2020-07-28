#include <algorithm>

#include "ArenaSize.h"

#include "BeingManager.h"
#include "GenerationSystem.h"
#include "GeneSystem.h"
#include "SpriteSystem.h"
#include "SurvivalSystem.h"
#include "TransformSystem.h"
#include "VelocitySystem.h"
#include "TextureSystem.h"

constexpr float DEPLETION_MODIFIER = 0.0000005f;

using SurvivalState = SurvivalComponent::SurvivalState;
using Handle = unsigned int;

bool SurvivalSystem::getIsAlive(const SurvivalComponent& component) const
{
	return component.isAlive;
}

SurvivalState SurvivalSystem::getSurvivalState(const SurvivalComponent& component) const
{
	return component.state;
}

void SurvivalSystem::setSurvivalState(SurvivalComponent& component, const SurvivalState state) const
{
	component.state = state;
}

void SurvivalSystem::setIsAlive(SurvivalComponent& component, const bool isAlive) const
{
	component.isAlive = isAlive;
}

void SurvivalSystem::setFullness(SurvivalComponent& component, float set) const
{
	component.fullness = set;
}

float SurvivalSystem::getBeingEnergy(const SurvivalComponent& component) const
{
	return component.energy;
}

float SurvivalSystem::getBeingFullness(const SurvivalComponent& component) const
{
	return component.fullness;
}

void SurvivalSystem::setStamina(SurvivalComponent& component, float set) const
{
	component.energy = set * 1000.0f;
}

void SurvivalSystem::update(BeingManager& beings, uint32_t dt)
{
	int waveState{ static_cast<int>(WaveState::NOTHING_ACTIVE) };
	waveOver = false;

	// Get handles of all beings which are food
	if (getFoodHandlesNeedUpdate())
	{
		foodHandles.clear();
		size_t poolSize = beings.pool.size();
		for (Handle beingHandle = 0; beingHandle < poolSize; beingHandle++)
		{
			if (!getIsAlive(beings.pool[beingHandle].survival))
			{
				foodHandles.push_back(beingHandle);
			}
		}

		setFoodHandlesNeedUpdate(false);
	}

	static unsigned int frameCount = 0;
	if (++frameCount % 20 != 0) return;
	if (frameCount > 1000) frameCount = 0;

	float depletion = DEPLETION_MODIFIER * static_cast<float>(dt) * velocitySystem.getPhysicsSpeedVal();

	const auto processSurvivalBehaviour = [this, &waveState, depletion, &beings](Being& being)
	{
		if (!getIsAlive(being.survival))
		{
			waveState = waveState | static_cast<int>(WaveState::FOOD_ACTIVE); //there IS food
			wait(being);
		}
		else if (getSurvivalState(being.survival) == SurvivalState::AWAITING)
		{
			wait(being);
		}
		else if (getSurvivalState(being.survival) == SurvivalState::SEARCHING)
		{
			if (depleteEnergy(being.survival, depletion))
			{
				killAnimal(being);
			}
			else
			{
				// SEEK FOOD
				const glm::vec2& beingPosition = transformSystem.getPos(being.transform);
				const glm::vec2 nearestFoodPosition = findNearestFoodToPoint(beingPosition, beings, foodHandles);
				if (nearestFoodPosition == beingPosition) // if no food found
				{
					velocitySystem.setVelocity(being.velocity, 0.0f);
				}
				else
				{
					const glm::vec2& directionToFood = glm::normalize(glm::vec2(nearestFoodPosition.x - beingPosition.x, nearestFoodPosition.y - beingPosition.y));
					velocitySystem.setDirection(being.velocity, directionToFood);
					velocitySystem.setVelocity(being.velocity, geneSystem.getTrait(being.gene, Trait::SPEED));
				}
				waveState = waveState | static_cast<int>(WaveState::SEARCHERS_ACTIVE); //there IS searchers
			}
		}
		else if (getSurvivalState(being.survival) == SurvivalState::RETURNING)
		{
			if (depleteEnergy(being.survival, depletion))
			{
				killAnimal(being);
			}
			else
			{
				// SEEK HOME 
				const glm::vec2& pos = transformSystem.getPos(being.transform);
				if (glm::length(pos) > ARENA_SIZE / 2.0f)
				{
					setSurvivalState(being.survival, SurvivalState::AWAITING);
				}
				else
				{
					velocitySystem.setVelocity(being.velocity, geneSystem.getTrait(being.gene, Trait::SPEED));
					//direction is normalised position, since we're just retreating from the centre.
					velocitySystem.setDirection(being.velocity, glm::normalize(pos));
				}
				waveState = waveState | static_cast<int>(WaveState::RETURNERS_ACTIVE); //there IS returners
			}
		}
	};

	for_each(begin(beings.pool), end(beings.pool), processSurvivalBehaviour);

	if (!(waveState & static_cast<int>(WaveState::FOOD_ACTIVE))) // if NO food
	{
		const auto killIfStillSearching = [this](Being& being)
		{
			if (getSurvivalState(being.survival) == SurvivalState::SEARCHING)
			{
				killAnimal(being);
			}
		};

		for_each(begin(beings.pool), end(beings.pool), killIfStillSearching);
	}

	if (!(waveState & static_cast<int>(WaveState::RETURNERS_ACTIVE)) && !(waveState & static_cast<int>(WaveState::SEARCHERS_ACTIVE))) // if NO returners and NO searchers
	{
		waveOver = true;
	}
}

glm::vec2 SurvivalSystem::findNearestFoodToPoint(const glm::vec2& point, const BeingManager& beings, const std::vector<Handle>& foodHandles) const
{
	glm::vec2 nearest(-FLT_MAX);
	float defaultClosest = glm::distance(nearest, point);
	float closestDist = defaultClosest;

	const auto compareToClosest = [&nearest, &closestDist, this, &beings, &point](Handle handle)
	{
		const glm::vec2& foodPos = transformSystem.getPos(beings.pool[handle].transform);
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

//true when energy is depleted
bool SurvivalSystem::depleteEnergy(SurvivalComponent& component, float depletion) const
{
	component.energy = std::max(component.energy - depletion, 0.0f);

	return component.energy == 0.0f;
}

void SurvivalSystem::wait(Being& being) const
{
	velocitySystem.setVelocity(being.velocity, 0);
}

void SurvivalSystem::killAnimal(Being& being)
{
	spriteSystem.setColour(being.sprite, DEAD_ANIMAL_COLOUR);
	spriteSystem.setTextureID(being.sprite, textureSystem.getTextureID(TextureSystem::Component::MEAT));
	setSurvivalState(being.survival, SurvivalState::AWAITING);
	setIsAlive(being.survival, false);
}