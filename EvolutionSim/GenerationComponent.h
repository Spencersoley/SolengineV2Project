#pragma once
#include "Handle.h"

class GenerationComponent
{
	template <class T> friend class GenerationSystemInterface;
	int waveCount{ 1 };
	int animalCount{ 0 };
	int plantCount{ 0 };
	int meatCount{ 0 };
	int toxicCount{ 0 };
	int totalActiveBeings{ 0 };
	bool isAvailableFood{ false };
	bool waveOver{ false };
	bool huntOver{ false };
};