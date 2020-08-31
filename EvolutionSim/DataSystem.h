#pragma once
struct DataPointEntity;
class DataComponent;

class DataSystem
{
public:
	void accumulateAllData(DataPointEntity& data) const;

	void halveDataResolution(DataPointEntity& data) const;

	void addPoint(DataComponent& data, float point) const;

	void clear(DataComponent& component) const;
	void reserveAdditional(DataComponent& component, const size_t extraCapacity) const;
	void reset(DataPointEntity& dataPoint) const;

private:
	void accumulateDataPoint(DataComponent& component, int resolution) const;
	void halveDataPointResolution(DataComponent& component) const;
};