#pragma once
#include <array>

#include "DataComponent.h"

class DataSystem
{
public:
	enum class Data
	{
		SPEED,
		STRENGTH,
		DIET,
		STAMINA,
		PLANT_POPULATION,
		ANIMAL_POPULATION,
		BEING_MAXIMUM,
		TRAIT_SIZE
	};

	const std::vector<float>& getData(Data dataHandle) const;
	void addPoint(Data dataHandle, const float dataPoint);
	void clear(Data dataHandle);
	void reserveAdditional(Data dataHandle, const size_t extraCapacity);
	void reset();

private:
	std::array<DataComponent, static_cast<int>(Data::TRAIT_SIZE)> components;
};