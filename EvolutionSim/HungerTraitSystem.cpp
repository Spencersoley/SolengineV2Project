#include <set>
#include "GameData.h"
#include "DefaultColours.h"
#include "SurvivalSystemSpecialization.h"
#include "SpriteSystemSpecialization.h"
#include "GeneSystemSpecialization.h"
#include "GenerationSystemSpecialization.h"
#include "TextureLoaderSystemSpecialization.h"
#include "TransformSystemSpecialization.h"
#include "FoodSystemSpecialization.h"
#include "AggressionTraitSystemSpecialization.h"
#include "DietTraitSystemSpecialization.h"
#include "HealthTraitSystemSpecialization.h"
#include "HungerTraitSystemSpecialization.h"
#include "IntelligenceTraitSystemSpecialization.h"
#include "StaminaTraitSystemSpecialization.h"
#include "SpeedTraitSystemSpecialization.h"

using SurvivalState = SurvivalComponent::SurvivalState;

constexpr float AGGRESSION_TRAIT_PREDATOR_MINIMUM_REQUIREMENT{ 0.6f };
constexpr float DIET_TRAIT_CARNIVORE_MINIMUM_REQUIREMENT{ 0.3f };
constexpr float DIET_TRAIT_HERBIOVE_MAXIMUM_REQUIREMENT{ 0.7f };
constexpr float FULLNESS_WHEN_FULL_MODIFIER = 1000.0f;
constexpr float POISONED_MEAT_INTELLIGENCE_CHECK = 0.4f;
constexpr float POISONED_PLANT_INTELLIGENCE_CHECK = 0.7f;

float HungerTraitSystem::getMaxFullness(const HungerTraitComponent& component) const
{
	return component.value * FULLNESS_WHEN_FULL_MODIFIER;
}

void HungerTraitSystem::seekNearestFood(BeingEntity& animal, const GameData& gameData) const
{
	float diet = DietTrait::System::getTraitValue(Gene::System::getDietTrait(animal.gene));
	const glm::vec2& animalPos = Transform::System::getPos(animal.transform);
	glm::vec2 nearest(-FLT_MAX);

	float defaultClosest = glm::distance(nearest, animalPos);
	float closestDist = defaultClosest;

	float intelligence = IntelligenceTrait::System::getTraitValue(Gene::System::getIntelligenceTrait(animal.gene));
	int meatCount = Generation::System::getMeatCount(gameData.generation.component);
	int plantCount = Generation::System::getPlantCount(gameData.generation.component);
	const auto compareFoodToClosest = [&nearest, &closestDist, &diet, &intelligence, &plantCount, &meatCount, &animalPos](const FoodEntity& food)
	{
		FoodType foodType = Food::System::getFoodType(food.food);
		if ((diet < DIET_TRAIT_HERBIOVE_MAXIMUM_REQUIREMENT && foodType == FoodType::PLANT) 
			|| (foodType == FoodType::MEAT && plantCount == 0)
			|| (foodType == FoodType::PLANT && meatCount == 0)
			|| diet > DIET_TRAIT_CARNIVORE_MINIMUM_REQUIREMENT && foodType == FoodType::MEAT)
		{
			const glm::vec2& foodPos = Transform::System::getPos(food.transform);
			const float dist = glm::distance(foodPos, animalPos);

			if (dist < closestDist)
			{
				if (Food::System::getIsToxic(food.food))
				{
					if ((foodType == FoodType::PLANT && intelligence >= POISONED_PLANT_INTELLIGENCE_CHECK) ||
						(foodType == FoodType::MEAT && intelligence >= POISONED_MEAT_INTELLIGENCE_CHECK))
					{
						return;
					}
				}
				nearest = foodPos;
				closestDist = dist;
			}
		}
	};

	std::for_each(cbegin(gameData.foodManager.pool), cend(gameData.foodManager.pool), compareFoodToClosest);

	const auto compareAnimalToClosest = [&nearest, &closestDist, &diet, &animal, &animalPos](const BeingEntity& being)
	{
		const glm::vec2& preyPos = Transform::System::getPos(being.transform);
		const float dist = glm::distance(preyPos, animalPos);

		if (dist < closestDist && dist != 0.0f)
		{
			if (!Gene::System::isSharedFamilyHistory(being.gene, animal.gene) 
				&& Survival::System::getSurvivalState(being.survival) == SurvivalState::SEARCHING)
			{
				nearest = preyPos;
				closestDist = dist;
			}
		}
	};

	float aggression = AggressionTrait::System::getTraitValue(Gene::System::getAggressionTrait(animal.gene));
	if (aggression > AGGRESSION_TRAIT_PREDATOR_MINIMUM_REQUIREMENT 
		|| (meatCount == 0 && plantCount == 0)) //if enough aggression or no food as an alternative
	{
		std::for_each(cbegin(gameData.beingManager.pool), cend(gameData.beingManager.pool), compareAnimalToClosest);
	}

	if (defaultClosest == closestDist) nearest = animalPos;

	if (glm::distance(nearest, animalPos) < 5) //no food, or food is close, stop moving
	{
		SpeedTrait::System::setIsMoving(Gene::System::getSpeedTrait(animal.gene), false);
	}
	else
	{
		const glm::vec2& directionToFood = glm::normalize(glm::vec2(nearest.x - animalPos.x, nearest.y - animalPos.y));
		SpeedTrait::System::setDirection(Gene::System::getSpeedTrait(animal.gene), directionToFood);
		SpeedTrait::System::setIsMoving(Gene::System::getSpeedTrait(animal.gene), true);
	}
}

void HungerTraitSystem::eatOnCollision(float foodMultiplier, Handle animalHandle, Handle foodHandle, GameData& gameData) const
{
	BeingEntity& being = gameData.beingManager.pool[animalHandle];
	FoodEntity& food = gameData.foodManager.pool[foodHandle];
	if (Survival::System::getSurvivalState(being.survival) == SurvivalState::SEARCHING)
	{
		// evaluate eat interaction through modification algorithm [could elaborate here]
		FoodComponent& foodComponent = food.food;
		if (Food::System::getIsToxic(foodComponent))
		{
			FoodType foodType = Food::System::getFoodType(foodComponent);
		
			if (const float intelligence = IntelligenceTrait::System::getTraitValue(Gene::System::getIntelligenceTrait(being.gene)); 
				foodType == FoodType::PLANT && intelligence >= POISONED_PLANT_INTELLIGENCE_CHECK ||
				foodType == FoodType::MEAT && intelligence >= POISONED_MEAT_INTELLIGENCE_CHECK)
			{
				return;
			}
	
			Survival::System::setIsPoisoned(being.survival, true);
			Food::System::setIsToxic(foodComponent, false);
		}
		const float maxFoodDepletable = Food::System::getSaturation(foodComponent);
		HungerTraitComponent& hungerComponent = Gene::System::getHungerTrait(being.gene);
		const float maxFullness = HungerTrait::System::getMaxFullness(hungerComponent);
		const float maxFoodConsumable = maxFullness - HungerTrait::System::getFullness(hungerComponent);
		const float dietaryBonus = DietTrait::System::getDietaryBonus(food, being);
		float modify = std::min(foodMultiplier * dietaryBonus, maxFoodConsumable);
		modify = std::min(modify, maxFoodDepletable);
		HungerTrait::System::setFullness(hungerComponent, HungerTrait::System::getFullness(hungerComponent) + modify);
		Food::System::depleteSaturation(foodComponent, modify);

		HealthTraitComponent& healthComponent = Gene::System::getHealthTrait(being.gene);
		const float maxHealth = HealthTrait::System::getHealthWhenFull(healthComponent);
		HealthTrait::System::setCurrentHealth(healthComponent, std::min(maxHealth, HealthTrait::System::getCurrentHealth(healthComponent) + modify));

		const float fullness = HungerTrait::System::getFullness(hungerComponent) / maxFullness;
		if (fullness >= 1.0f)
		{
			Survival::System::setSurvivalState(being.survival, SurvivalState::RETURNING); // completely full
			Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::BEING_8, gameData));
		}
		else if (fullness >= 0.87f)
		{
			Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::BEING_7, gameData));
		}
		else if (fullness >= 0.74f)
		{
			Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::BEING_6, gameData));
		}
		else if (fullness >= 0.61f)
		{
			Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::BEING_5, gameData));
		}
		else if (fullness >= 0.48f)
		{
			Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::BEING_4, gameData));
		}
		else if (fullness >= 0.35f)
		{
			Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::BEING_3, gameData));
		}
		else if (fullness >= 0.22f)
		{
			Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::BEING_2, gameData));
		}
		else if (fullness >= 0.10f)
		{
			Sprite::System::setTextureID(being.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::BEING_1, gameData));
		}
	}
}
