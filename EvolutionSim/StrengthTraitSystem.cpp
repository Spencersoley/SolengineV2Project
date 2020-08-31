#include "StrengthTraitSystemSpecialization.h"
#include "GeneSystemSpecialization.h"
#include "TransformSystemSpecialization.h"
#include "GameData.h"

void StrengthTraitSystem::resolveStrengthCollision(Handle animalHandleA, Handle animalHandleB, GameData& gameData, glm::vec2& collisionDepthVec)
{
	Transform::System::translate(
		gameData.beingManager.pool[animalHandleA].transform,
		collisionDepthVec);
	Transform::System::translate(
		gameData.beingManager.pool[animalHandleB].transform,
		-collisionDepthVec);
}

std::tuple<float, float> StrengthTraitSystem::getCombatBonus(Handle animalHandleA, Handle animalHandleB, GameData& gameData)
{
	BeingEntity& animalA = gameData.beingManager.pool[animalHandleA];
	float strengthA = StrengthTrait::System::getTraitValue(Gene::System::getStrengthTrait(animalA.gene));
	BeingEntity& animalB = gameData.beingManager.pool[animalHandleB];
	float strengthB = StrengthTrait::System::getTraitValue(Gene::System::getStrengthTrait(animalB.gene));

	return { strengthA, strengthB };
}

