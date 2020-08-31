#include <algorithm>
#include "DataSystemSpecialization.h"
#include "DataPointEntity.h"

void DataSystem::accumulateAllData(DataPointEntity& data) const
{
	int counter = ++data.currentResolutionStage;

	if (data.currentResolutionStage == data.dataResolution)
	{
		int resolution = data.dataResolution;
		data.currentResolutionStage = 0;
		accumulateDataPoint(data.aggression, resolution);
		accumulateDataPoint(data.diet, resolution);
		accumulateDataPoint(data.health, resolution);
		accumulateDataPoint(data.hunger, resolution);
		accumulateDataPoint(data.intelligence, resolution);
		accumulateDataPoint(data.speed, resolution);
		accumulateDataPoint(data.stamina, resolution);
		accumulateDataPoint(data.strength, resolution);

		accumulateDataPoint(data.animalPopulation, resolution);
		accumulateDataPoint(data.plantPopulation, resolution);
		accumulateDataPoint(data.causeOfDeathPoison, resolution);
		accumulateDataPoint(data.causeOfDeathPredator, resolution);
		accumulateDataPoint(data.causeOfDeathStarvation, resolution);

		accumulateDataPoint(data.waveCount, resolution);
	}

	if (Data::System::getData(data.speed).size() >= data.graphCapacity)
	{
		data.dataResolution *= 2; //stored inverted (resolution actually halves here)
		halveDataResolution(data);
		data.currentResolutionStage = 0;
	}
}

void DataSystem::accumulateDataPoint(DataComponent& component, int resolution) const
{
	Data::System::addPoint(component, Data::System::getCurrentPoint(component) / resolution);
	Data::System::setCurrentPoint(component, 0.0f);
}

// halves the datasets
void DataSystem::halveDataResolution(DataPointEntity& data) const
{
	halveDataPointResolution(data.aggression);
	halveDataPointResolution(data.diet);
	halveDataPointResolution(data.health);
	halveDataPointResolution(data.hunger);
	halveDataPointResolution(data.intelligence);
	halveDataPointResolution(data.speed);
	halveDataPointResolution(data.stamina);
	halveDataPointResolution(data.strength);

	halveDataPointResolution(data.animalPopulation);
	halveDataPointResolution(data.plantPopulation);
	halveDataPointResolution(data.causeOfDeathPoison);
	halveDataPointResolution(data.causeOfDeathPredator);
	halveDataPointResolution(data.causeOfDeathStarvation);

	halveDataPointResolution(data.waveCount);
}

void DataSystem::addPoint(DataComponent& data, float point) const
{
	Data::System::getData(data).push_back(point);
}

void DataSystem::halveDataPointResolution(DataComponent& component) const
{
	std::vector<float>& dataPoints = Data::System::getData(component);
	size_t dataSize = dataPoints.size();
	for (size_t i = 0, j = 0; j + 1 < dataSize; ++i, j += 2)
	{
		dataPoints[i] = (dataPoints[j] + dataPoints[j + 1]) / 2.0f;
	}
	dataPoints.erase(dataPoints.cbegin() + (dataPoints.size() / 2), dataPoints.cend());
}

void DataSystem::clear(DataComponent& component) const
{
	Data::System::getData(component) = { 0.5f };
	Data::System::setCurrentPoint(component, 0.0f);
}

void DataSystem::reserveAdditional(DataComponent& component, const size_t extraCapacity) const
{
	Data::System::getData(component).reserve(Data::System::getData(component).capacity() + (extraCapacity * sizeof(float)));
}

void DataSystem::reset(DataPointEntity& dataPoint) const
{
	Data::System::clear(dataPoint.aggression);
	Data::System::clear(dataPoint.diet);
	Data::System::clear(dataPoint.health);
	Data::System::clear(dataPoint.hunger);
	Data::System::clear(dataPoint.intelligence);
	Data::System::clear(dataPoint.speed);
	Data::System::clear(dataPoint.stamina);
	Data::System::clear(dataPoint.strength);

	Data::System::clear(dataPoint.animalPopulation);
	Data::System::clear(dataPoint.plantPopulation);
	Data::System::clear(dataPoint.beingMaximum);

	Data::System::clear(dataPoint.causeOfDeathPoison);
	Data::System::clear(dataPoint.causeOfDeathPredator);
	Data::System::clear(dataPoint.causeOfDeathStarvation);

	Data::System::clear(dataPoint.waveCount);
	dataPoint.currentResolutionStage = 0;
	dataPoint.dataResolution = 1;
}