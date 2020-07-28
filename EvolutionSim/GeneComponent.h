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

private:
	friend class GeneSystem;
	friend struct DuplicationData;
	BeingType beingType{ BeingType::PLANT };
	std::array<float, static_cast<int>(Trait::TRAIT_COUNT)> traits{ 0.5f, 0.5f, 0.5f, 0.5f, 0.5f };
};