#pragma once
#include <set>
#include <chrono>
struct GameData;
struct BeingEntity;

class ColliderSystem
{
	using Handle = unsigned int;

public:
	void update(const std::chrono::microseconds& dt, GameData& gameData);
};