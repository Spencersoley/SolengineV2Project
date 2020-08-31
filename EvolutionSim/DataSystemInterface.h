#pragma once
#include "DataComponent.h"
struct DataPointEntity;

template <class Implementation>
class DataSystemInterface
{
public:
	static std::vector<float>& getData(DataComponent& component) { return component.points; }
	
	static float getCurrentPoint(DataComponent& component) { return component.currentPoint; }
	static void setCurrentPoint(DataComponent& component, float set) { component.currentPoint = set; }
	static void modifyCurrentPoint(DataComponent& component, float modify) { component.currentPoint += modify; }

	static void addPoint(DataComponent& component, float point)
	{
		static Implementation system;
		system.addPoint(component, point);
	}

	static void accumulateAllData(DataPointEntity& data)
	{
		static Implementation system;
		system.accumulateAllData(data);
	}

	static void halveDataResolution(DataPointEntity& data)
	{
		static Implementation system;
		system.halveDataResolution(data);
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

	static void reset(DataPointEntity& dataPoint)
	{
		static Implementation system;
		system.reset(dataPoint);
	}
};