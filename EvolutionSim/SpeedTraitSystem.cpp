#include "GameData.h"
#include "SpeedTraitSystemSpecialization.h"
#include "TransformSystemSpecialization.h"
#include "GeneSystemSpecialization.h"

constexpr float VELOCITY_MODIFIER = 0.000005f;

void SpeedTraitSystem::update(const std::chrono::microseconds& dt, GameData& gameData) const
{
	float physicsSpeed = gameData.configurableSettings.physicsSpeed.get();
	float adjustedDT = physicsSpeed * dt.count() * VELOCITY_MODIFIER;

	bool bounded = gameData.configurableSettings.physicsSpeed.get();
	const auto updatePosition = [adjustedDT, bounded, &gameData](BeingEntity& being)
	{
		if (SpeedTrait::System::getIsMoving(Gene::System::getSpeedTrait(being.gene)))
		{
			Transform::System::translate(
				being.transform,
				SpeedTrait::System::getTraitValue(Gene::System::getSpeedTrait(being.gene))
				* adjustedDT
				* SpeedTrait::System::getDirection(Gene::System::getSpeedTrait(being.gene)));
		}
	};

	Manager<BeingEntity>& beingManager = gameData.beingManager;
	std::for_each(begin(beingManager.pool), end(beingManager.pool), updatePosition);
}