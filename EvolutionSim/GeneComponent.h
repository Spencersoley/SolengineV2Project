#pragma once
#include <array>

class GeneComponent
{
public:
	enum class Trait
	{
		SPEED,
		DIET,
		STRENGTH,
		STAMINA,
		HUNGER,
		TRAIT_COUNT
	};

	enum class BeingType
	{
		PLANT = 1 << 0,
		ANIMAL = 1 << 1
	};

	GeneComponent() {};
	GeneComponent(
		BeingType type,
		float beingSpeed,
		float beingDiet,
		float beingStrength,
		float beingStamina,
		float beingHunger
	) :
		beingType(type)
	{
		traits[static_cast<int>(Trait::SPEED)]    = beingSpeed;
		traits[static_cast<int>(Trait::DIET)]     = beingDiet;
		traits[static_cast<int>(Trait::STRENGTH)] = beingStrength;
		traits[static_cast<int>(Trait::STAMINA)]  = beingStamina;
		traits[static_cast<int>(Trait::HUNGER)]   = beingHunger;
	}

private:
	friend class GeneSystem;
	friend struct DuplicationData;
	BeingType beingType{ BeingType::PLANT };
	std::array<float, static_cast<int>(Trait::TRAIT_COUNT)> traits{ 0.5f, 0.5f, 0.5f, 0.5f, 0.5f };
};