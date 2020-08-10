#pragma once
using Handle = unsigned int;

class GenerationComponent
{
	template <class T> friend class GenerationSystemInterface;

	int foodPerGeneration{ 0 };
	int waveCount{ 1 };
	int animalCount{ 0 };
	int plantCount{ 0 };
	int totalActiveBeings{ 0 };
	float physicsSpeed{ 10.0f };

	std::vector<Handle> foodHandles{};

	bool foodHandlesNeedUpdate{ true };
	bool waveOver{ false };
	bool huntOver{ false };
};