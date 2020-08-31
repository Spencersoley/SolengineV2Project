#include "DietTraitSystemSpecialization.h"
#include "GeneSystemSpecialization.h"
#include "FoodSystemSpecialization.h"
#include "BeingEntity.h"
#include "FoodEntity.h"

constexpr float MEAT_BONUS{ 3.0f };
constexpr float PLANT_BONUS{ 1.0f };

float DietTraitSystem::getDietaryBonus(FoodEntity& food, BeingEntity& being) const
{
	const DietTraitComponent& diet = Gene::System::getDietTrait(being.gene);
	const float dietValue = DietTrait::System::getTraitValue(diet);
	FoodType foodType = Food::System::getFoodType(food.food);
	
	if (foodType == FoodType::PLANT)
	{
	    return PLANT_BONUS * static_cast<float>(pow((1.0 - dietValue), 2.0));
	}
	else // FoodType::Meat;
	{  
		return MEAT_BONUS * static_cast<float>(pow(dietValue, 2.0));
	}
}