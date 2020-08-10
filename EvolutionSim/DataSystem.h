#pragma once

class DataComponent;

class DataSystem
{
public:
	void addPoint(DataComponent& component, const float dataPoint);
	void clear(DataComponent& component);
	void reserveAdditional(DataComponent& component, const size_t extraCapacity);
};