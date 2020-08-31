#pragma once

enum class FoodType
{
	PLANT,
	MEAT
};

class FoodComponent
{
	friend class FoodSystem;
	float saturation{};
	FoodType type{ FoodType::PLANT };
	bool isToxic{ false };
};