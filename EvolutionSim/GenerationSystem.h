#pragma once
#include <set>
#include <chrono>
#include "Handle.h"
struct GameData;
struct BeingEntity;
namespace SolengineV2
{
	class RandomNumberGenerator;
}

class GenerationSystem
{
public:
	void begin(GameData& gameData, SolengineV2::RandomNumberGenerator& rng) const;
	void update(GameData& gameData, SolengineV2::RandomNumberGenerator& rng, const std::chrono::microseconds& deltaTime) const;

private:
	void newWave(GameData& gameData, SolengineV2::RandomNumberGenerator& rng) const;

	void killAnimal(BeingEntity& being, Handle handle, GameData& gameData) const;
};