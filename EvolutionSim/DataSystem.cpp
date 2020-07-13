#include "DataSystem.h"

void DataSystem::reset()
{
	const auto reset_points = [](DataComponent& component) { component.points = { 0.5f }; };
	std::for_each(begin(components), end(components), reset_points);
}
