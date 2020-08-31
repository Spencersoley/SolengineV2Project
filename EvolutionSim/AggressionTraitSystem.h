#pragma once
#include "Handle.h"
struct GameData;

class AggressionTraitSystem
{
public:
	void resolveAggressionTest(Handle animalHandleA, Handle animalHandleB, float combatMultiplier, GameData& gameData);
};