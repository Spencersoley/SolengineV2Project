#pragma once

namespace Gene
{
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
}
