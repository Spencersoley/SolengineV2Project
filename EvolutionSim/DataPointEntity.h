#pragma once
#include "DataComponent.h"

//some systems depend on this implementation
struct DataPointEntity
{
	DataComponent aggression;
	DataComponent diet;
	DataComponent health;
	DataComponent hunger;
	DataComponent intelligence;
	DataComponent speed;
	DataComponent stamina;
	DataComponent strength;
	DataComponent plantPopulation;
	DataComponent animalPopulation;
	DataComponent beingMaximum;
	DataComponent deathMaximum;

	DataComponent waveCount;

	DataComponent causeOfDeathPoison;
	DataComponent causeOfDeathPredator;
	DataComponent causeOfDeathStarvation;

	int dataResolution{ 1 };
	int currentResolutionStage{ 0 };
	size_t graphCapacity{ 100 };
};