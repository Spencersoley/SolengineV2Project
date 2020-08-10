#pragma once
#include "DataComponent.h"

template <class Implementation>
class DataSystemInterface
{
public:
	static std::vector<float>& getData(DataComponent& component) { return component.points; }
	
	static void addPoint(DataComponent& component, const float dataPoint)
	{
		static Implementation system;
		system.addPoint(component, dataPoint);
	}

	static void clear(DataComponent& component)
	{
		static Implementation system;
		system.clear(component);
	}

	static void reserveAdditional(DataComponent& component, const size_t extraCapacity)
	{
		static Implementation system;
		system.reserveAdditional(component, extraCapacity);
	}
};