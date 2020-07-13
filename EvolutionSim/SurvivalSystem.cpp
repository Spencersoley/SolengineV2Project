#include "SurvivalSystem.h"
#include "PartitionSystem.h"
#include "VelocitySystem.h"
#include "TransformSystem.h"
#include "SpriteSystem.h"
#include "BeingManager.h"
#include "GenerationSystem.h"
#include "GeneSystem.h"

constexpr float DEPLETION_MODIFIER = 0.000000001f;

void SurvivalSystem::eatFood(SurvivalComponent& survivalA, SurvivalComponent& survivalB, const float maxFullness, float modify) const
{
	float maxFoodConsumable = maxFullness - getBeingFullness(survivalA);
	float maxFoodDepletable = getBeingFullness(survivalB);
	modify = std::min(modify, maxFoodConsumable);
	modify = std::min(modify, maxFoodDepletable);
	modifyFullness(survivalA, modify);
	modifyFullness(survivalB, -modify);
}

void SurvivalSystem::process(BeingManager& beings, long long dt)
{
	dt *= velocitySystem.getPhysicsSpeedVal();

	int waveState{ static_cast<int>(WaveState::NOTHING_ACTIVE) };
	waveOver = false;
	float depletion = DEPLETION_MODIFIER * dt;
	std::vector<uint32_t> foodHandles = partitionSystem.processFoodHandles(beings);

	for (uint32_t beingHandle = 0; beingHandle < beings.getSize(); beingHandle++)
	{
		SurvivalComponent& survival = beings.getSurvivalComponent(beingHandle);
		if (!getIsAlive(survival))
		{
			waveState = waveState | static_cast<int>(WaveState::FOOD_ACTIVE); //there IS food
			wait(beings, beingHandle);
		}
		else if (survival.state == SurvivalState::AWAITING)
		{
			wait(beings, beingHandle);
		}
		else if (survival.state == SurvivalState::SEARCHING)
		{
			if (depleteEnergy(survival, depletion))
			{
				spriteSystem.setColour(beings.getSpriteComponent(beingHandle), DEAD_ANIMAL_COLOUR);
				setSurvivalState(survival, SurvivalState::AWAITING);
				setIsAlive(survival, false);
			}
			else
			{
				waveState = waveState | static_cast<int>(WaveState::SEARCHERS_ACTIVE); //there IS searchers
				seekFood(beings, beingHandle, foodHandles); //repeatedly seeks nearest food
			}
		}
		else if (survival.state == SurvivalState::RETURNING)
		{
			if (depleteEnergy(survival, depletion))
			{
				spriteSystem.setColour(beings.getSpriteComponent(beingHandle), DEAD_ANIMAL_COLOUR);
				setSurvivalState(survival, SurvivalState::AWAITING);
				setIsAlive(survival, false);
			}
			else
			{
				seekHome(beings, beingHandle);
				waveState = waveState | static_cast<int>(WaveState::RETURNERS_ACTIVE); //there IS returners
			}
		}
	}

	if (!(waveState & static_cast<int>(WaveState::FOOD_ACTIVE))) // if NO food
	{
		killSearchers(beings);
	}

	if (!(waveState & static_cast<int>(WaveState::RETURNERS_ACTIVE)) && !(waveState & static_cast<int>(WaveState::SEARCHERS_ACTIVE))) // if NO returners and NO searchers
	{
		waveOver = true;
	}
}

std::vector<uint32_t> SurvivalSystem::compileToDelete(const BeingManager& beings)
{
	std::vector<uint32_t> compiled{};// (beings.getSize());
	for (uint32_t beingHandle = 0; beingHandle < beings.getSize(); beingHandle++)
	{
		if (!getExists(beings.getSurvivalComponent(beingHandle)))
		{
			compiled.push_back(beingHandle);
		}
	}

	return compiled;
}

// If anything is not alive, it won't mutate - therefore it's food
void SurvivalSystem::replicate(const BeingManager& beings, std::vector<DuplicationData>& duplicationData) const
{
	for (uint32_t beingHandle = 0; beingHandle < beings.getSize(); beingHandle++)
	{
		const SurvivalComponent& survival = beings.getSurvivalComponent(beingHandle);
		const GeneComponent& gene = beings.getGeneComponent(beingHandle);
		const TransformComponent& transform = beings.getTransformComponent(beingHandle);
		if (getIsAlive(survival))
		{
			duplicationData.push_back(DuplicationData{ transformSystem.getPos(transform), true, 0.0f, geneSystem.mutate(gene) });
			duplicationData.push_back(DuplicationData{ transformSystem.getPos(transform), true, 0.0f, geneSystem.mutate(gene) });
		}
		else
		{
			duplicationData.push_back(DuplicationData{ transformSystem.getPos(transform), false, getBeingFullness(survival), gene });
		}
	}
}

void SurvivalSystem::eating(float dt, BeingManager& beings, const uint32_t animalHandle, const uint32_t foodHandle)
{
	SurvivalComponent& survivalA = beings.getSurvivalComponent(animalHandle);
	if (getSurvivalState(survivalA) == SurvivalState::SEARCHING)
	{
		const GeneComponent& geneB = beings.getGeneComponent(foodHandle);
		const GeneComponent& geneA = beings.getGeneComponent(animalHandle);

		float dietBonus = evaluateDietaryBonus(geneSystem.getBeingType(geneB), geneSystem.getTrait(geneA, Trait::DIET));
		float maxFullness = geneSystem.getTrait(geneA, Trait::HUNGER) * 1000.0f;
		SurvivalComponent& survivalB = beings.getSurvivalComponent(foodHandle);
		eatFood(survivalA, survivalB, maxFullness, dt * dietBonus);
		if (getBeingFullness(survivalB) <= 0)
		{
			setExists(beings.getSurvivalComponent(foodHandle), false); //completely eaten
		}

		if (getBeingFullness(survivalA) >= maxFullness)
		{
			setSurvivalState(survivalA, SurvivalState::RETURNING); // completely full
		}
	}
}

glm::vec2 SurvivalSystem::findNearestFoodToPoint(const glm::vec2& point, const BeingManager& beings, const std::vector<uint32_t>& foodHandles) const
{
	glm::vec2 nearest(-FLT_MAX);
	float defaultClosest = glm::distance(nearest, point);
	float closestDist = defaultClosest;

	const auto checkForClosest = [&nearest, &closestDist, this, &beings, &point](uint32_t handle)
	{
		const glm::vec2& foodPos = transformSystem.getPos(beings.getTransformComponent(handle));
		const float dist = glm::distance(foodPos, point);
		if (dist < closestDist)
		{
			nearest = foodPos;
			closestDist = dist;
		}
	};

	std::for_each(cbegin(foodHandles), cend(foodHandles), checkForClosest);

	return (defaultClosest == closestDist) ? point : nearest;
}

inline float SurvivalSystem::evaluateDietaryBonus(const BeingType foodType, float dietType) const
{
	if (foodType == BeingType::PLANT)
	{
		return pow((1 - dietType) * 2, 4);
	}
	else
	{
		return pow(dietType * 2, 4);
	}
}

void SurvivalSystem::seekFood(BeingManager& beings, const uint32_t beingHandle, const std::vector<uint32_t>& foodHandles) const
{
	const glm::vec2& beingPosition = transformSystem.getPos(beings.getTransformComponent(beingHandle));
	const glm::vec2 nearestFoodPosition = findNearestFoodToPoint(beingPosition, beings, foodHandles);

	VelocityComponent& velocityComponent = beings.getVelocityComponent(beingHandle);
	if (nearestFoodPosition == beingPosition) // if no food found
	{
		velocitySystem.setVelocity(velocityComponent, 0.0f);
	}
	else
	{
		const glm::vec2& directionToFood = glm::normalize(glm::vec2(nearestFoodPosition.x - beingPosition.x, nearestFoodPosition.y - beingPosition.y));

		velocitySystem.setDirection(velocityComponent, directionToFood);
		const GeneComponent& gene = beings.getGeneComponent(beingHandle);
		velocitySystem.setVelocity(velocityComponent, geneSystem.getTrait(gene, Trait::SPEED));
	}
}

void SurvivalSystem::seekHome(BeingManager& beings, const uint32_t beingHandle) const
{
	const TransformComponent& transform = beings.getTransformComponent(beingHandle);
	const glm::vec2& pos = transformSystem.getPos(transform);
	if (glm::length(pos) > ARENA_SIZE / 2.0f)
	{
		setSurvivalState(beings.getSurvivalComponent(beingHandle), SurvivalState::AWAITING);
	}
	else
	{
		VelocityComponent& velocity = beings.getVelocityComponent(beingHandle);
		const GeneComponent& gene = beings.getGeneComponent(beingHandle);
		velocitySystem.setVelocity(velocity, geneSystem.getTrait(gene, Trait::SPEED));
		//direction is normalised position, since we're just retreating from the centre.
		velocitySystem.setDirection(velocity, glm::normalize(pos));
	}
}

//true when energy is depleted
bool SurvivalSystem::depleteEnergy(SurvivalComponent& component, float depletion) const
{
	component.energy -= depletion;

	if (component.energy < 0.0f)
	{
		component.energy = 0.0f;
		return true;
	}
	else
	{
		return false;
	}
}

void SurvivalSystem::wait(BeingManager& beings, const uint32_t beingHandle) const
{
	velocitySystem.setVelocity(beings.getVelocityComponent(beingHandle), 0);
}

void SurvivalSystem::killSearchers(BeingManager& beings) const
{
	for (uint32_t beingHandle = 0; beingHandle < beings.getSize(); beingHandle++)
	{
		SurvivalComponent& survival = beings.getSurvivalComponent(beingHandle);
		if (getSurvivalState(survival) == SurvivalState::SEARCHING)
		{
			spriteSystem.setColour(beings.getSpriteComponent(beingHandle), DEAD_ANIMAL_COLOUR);
			setSurvivalState(survival, SurvivalState::AWAITING);
			setIsAlive(survival, false);
		}
	}
}
