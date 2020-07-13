#pragma once
#include <vector>
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

	const DataComponent& getCComponent(Data dataHandle) const { return components[static_cast<int>(dataHandle)]; }
	DataComponent& getComponent(Data dataHandle) { return components[static_cast<int>(dataHandle)]; }
	const std::vector<float>& getData(Data dataHandle) const { return getCComponent(dataHandle).points; }
	void addPoint(Data dataHandle, const float dataPoint) { getComponent(dataHandle).points.push_back(dataPoint); }
	void clear(Data dataHandle) { getComponent(dataHandle).points = { 0.5f }; }

	void reset();

private:
	std::array<DataComponent, static_cast<int>(Data::TRAIT_SIZE)> components;
};