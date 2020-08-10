#pragma once
#include <array>
#include "DataComponent.h"

//some systems depend on this implementation
struct DataPointManager
{
	DataComponent speed;
	DataComponent strength;
	DataComponent stamina;
	DataComponent diet;
	DataComponent plantPopulation;
	DataComponent animalPopulation;
	DataComponent beingMaximum;
};