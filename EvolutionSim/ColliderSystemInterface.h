#pragma once
#include <chrono>
#include "Handle.h"
struct GameData;

template <class Implementation>
class ColliderSystemInterface
{
public:
	static void update(const std::chrono::microseconds dt, GameData& gameData)
	{
		static Implementation system;
		system.update(dt, gameData);
	}
};