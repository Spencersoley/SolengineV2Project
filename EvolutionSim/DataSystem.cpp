#include "DataSystem.h"

const std::vector<float>& DataSystem::getData(Data dataHandle) const
{
	return components[static_cast<int>(dataHandle)].points;
}

void DataSystem::addPoint(Data dataHandle, const float dataPoint)
{
	components[static_cast<int>(dataHandle)].points.push_back(dataPoint);
}

void DataSystem::clear(Data dataHandle)
{
	components[static_cast<int>(dataHandle)].points = { 0.5f };
}

void DataSystem::reserveAdditional(Data dataHandle, const size_t extraCapacity)
{
	DataComponent& component = components[static_cast<int>(dataHandle)];
	component.points.reserve(component.points.capacity() + (extraCapacity * sizeof(float)));
}

void DataSystem::reset()
{
	const auto reset_points = [](DataComponent& component) { component.points = { 0.5f }; };
	std::for_each(begin(components), end(components), reset_points);
}
