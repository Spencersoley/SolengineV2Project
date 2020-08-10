#include "DataSystemImplementation.h"
#include <algorithm>

void DataSystem::addPoint(DataComponent& component, const float dataPoint)
{
	Data::System::getData(component).push_back(dataPoint);
}

void DataSystem::clear(DataComponent& component)
{
	Data::System::getData(component) = { 0.5f };
}

void DataSystem::reserveAdditional(DataComponent& component, const size_t extraCapacity)
{
	Data::System::getData(component).reserve(Data::System::getData(component).capacity() + (extraCapacity * sizeof(float)));
}
