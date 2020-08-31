#include "GameData.h"
#include "DefaultColours.h"
#include "OverlaySystemSpecialization.h"
#include "SpriteSystemSpecialization.h"
#include "GeneSystemSpecialization.h"
#include "AggressionTraitSystemSpecialization.h"
#include "DietTraitSystemSpecialization.h"
#include "HealthTraitSystemSpecialization.h"
#include "HungerTraitSystemSpecialization.h"
#include "IntelligenceTraitSystemSpecialization.h"
#include "SpeedTraitSystemSpecialization.h"
#include "StaminaTraitSystemSpecialization.h"
#include "StrengthTraitSystemSpecialization.h"

using Colour = SolengineV2::Colour;

constexpr Colour LIGHT_GREENBLUE(126, 241, 175, 255);
constexpr Colour BLUE(22, 4, 255, 255);

constexpr Colour LIGHT_YELLOW(243, 237, 139, 255);
constexpr Colour RED(255, 0, 0, 255);

constexpr Colour PURPLE(40, 20, 58, 255);
constexpr Colour YELLOW(255, 255, 0, 255);

constexpr Colour DEEP_GREEN(43, 113, 40, 255);
constexpr Colour GREEN(0, 255, 0, 255);

inline Colour triLerpRGB(const Colour& a, const Colour& b, const Colour& c, float traitValue0to1, float threshold)
{
	if (traitValue0to1 <= threshold)
	{
		return Colour
		(
			static_cast<GLubyte>(a.r + (b.r - a.r) * traitValue0to1 * 2.0f),
			static_cast<GLubyte>(a.g + (b.g - a.g) * traitValue0to1 * 2.0f),
			static_cast<GLubyte>(a.b + (b.b - a.b) * traitValue0to1 * 2.0f),
			255
		);
	}
	else
	{
		return Colour
		(
			static_cast<GLubyte>(b.r + (c.r - b.r) * (traitValue0to1 - 0.5f) * 2.0f),
			static_cast<GLubyte>(b.g + (c.g - b.g) * (traitValue0to1 - 0.5f) * 2.0f),
			static_cast<GLubyte>(b.b + (c.b - b.b) * (traitValue0to1 - 0.5f) * 2.0f),
			255
		);
	}
}

inline Colour biLerpRGB(const Colour& a, const Colour& b, float traitValue0to1)
{
	return Colour
	(
		static_cast<GLubyte>(static_cast<float>(a.r + (b.r - a.r) * traitValue0to1)),
		static_cast<GLubyte>(static_cast<float>(a.g + (b.g - a.g) * traitValue0to1)),
		static_cast<GLubyte>(static_cast<float>(a.b + (b.b - a.b) * traitValue0to1)),
		255
	);
}

inline Colour alphaLerp(const Colour& a, float traitValue0to1)
{
	return Colour{ static_cast<GLubyte>(traitValue0to1 * a.r), static_cast<GLubyte>(traitValue0to1 * a.g), static_cast<GLubyte>(traitValue0to1 * a.b), 255 };
}

void OverlaySystem::updateOverlay(GameData& gameData) const
{
	switch (Overlay::System::getOverlayMode(gameData.overlayConfiguration.component))
	{
	case OverlayMode::DEFAULT:
	{
		const auto setColourToDefault = [](BeingEntity& being)
		{
			Sprite::System::setColour(being.sprite, COLOUR::ANIMAL);
		};

		std::for_each(begin(gameData.beingManager.pool), end(gameData.beingManager.pool), setColourToDefault);
		break;
	}
	case OverlayMode::AGGRESSION:
	{
		const auto setColourBasedOnAggression = [](BeingEntity& being)
		{
			float aggressionTrait = AggressionTrait::System::getTraitValue(Gene::System::getAggressionTrait(being.gene));
			Sprite::System::setColour(being.sprite, triLerpRGB(RED, YELLOW, GREEN, aggressionTrait, 0.5f));
		};

		std::for_each(begin(gameData.beingManager.pool), end(gameData.beingManager.pool), setColourBasedOnAggression);
		break;
	}
	case OverlayMode::DIET:
	{
		const auto setColourBasedOnDiet = [](BeingEntity& being)
		{
			float dietTrait = DietTrait::System::getTraitValue(Gene::System::getDietTrait(being.gene));
			Sprite::System::setColour(being.sprite, triLerpRGB(RED, YELLOW, GREEN, dietTrait, 0.5f));
		};

		std::for_each(begin(gameData.beingManager.pool), end(gameData.beingManager.pool), setColourBasedOnDiet);
		break;
	}
	case OverlayMode::HEALTH:
	{
		const auto setColourBasedOnHealth = [](BeingEntity& being)
		{
			float healthTrait = HealthTrait::System::getTraitValue(Gene::System::getHealthTrait(being.gene));
			Sprite::System::setColour(being.sprite, triLerpRGB(RED, YELLOW, GREEN, healthTrait, 0.5f));
		};

		std::for_each(begin(gameData.beingManager.pool), end(gameData.beingManager.pool), setColourBasedOnHealth);
		break;
	}
	case OverlayMode::HUNGER:
	{
		const auto setColourBasedOnHunger = [](BeingEntity& being)
		{
			float hungerTrait = HungerTrait::System::getTraitValue(Gene::System::getHungerTrait(being.gene));
			Sprite::System::setColour(being.sprite, triLerpRGB(RED, YELLOW, GREEN, hungerTrait, 0.5f));
		};

		std::for_each(begin(gameData.beingManager.pool), end(gameData.beingManager.pool), setColourBasedOnHunger);
		break;
	}
	case OverlayMode::INTELLIGENCE: 
	{
		const auto setColourBasedOnIntelligence = [](BeingEntity& being)
		{
			float intelligenceTrait = IntelligenceTrait::System::getTraitValue(Gene::System::getIntelligenceTrait(being.gene));
			Sprite::System::setColour(being.sprite, triLerpRGB(RED, YELLOW, GREEN, intelligenceTrait, 0.5f));
		};

		std::for_each(begin(gameData.beingManager.pool), end(gameData.beingManager.pool), setColourBasedOnIntelligence);
		break;
	}
	case OverlayMode::SPEED:
	{
		const auto setColourBasedOnSpeed = [](BeingEntity& being)
		{
			float speedTrait = SpeedTrait::System::getTraitValue(Gene::System::getSpeedTrait(being.gene));
			Sprite::System::setColour(being.sprite, triLerpRGB(RED, YELLOW, GREEN, speedTrait, 0.5f));
		};

		std::for_each(begin(gameData.beingManager.pool), end(gameData.beingManager.pool), setColourBasedOnSpeed);
		break;
	}
	case OverlayMode::STAMINA:
	{
		const auto setColourBasedOnStamina = [](BeingEntity& being)
		{
			float staminaTrait = StaminaTrait::System::getTraitValue(Gene::System::getStaminaTrait(being.gene));
			Sprite::System::setColour(being.sprite, triLerpRGB(RED, YELLOW, GREEN, staminaTrait, 0.5f));
		};

		std::for_each(begin(gameData.beingManager.pool), end(gameData.beingManager.pool), setColourBasedOnStamina);
		break;
	}
	case OverlayMode::STRENGTH:
	{
		const auto setColourBasedOnStrength = [](BeingEntity& being)
		{
			float strengthTrait = StrengthTrait::System::getTraitValue(Gene::System::getStrengthTrait(being.gene));
			Sprite::System::setColour(being.sprite, triLerpRGB(RED, YELLOW, GREEN, strengthTrait, 0.5f));
		};

		std::for_each(begin(gameData.beingManager.pool), end(gameData.beingManager.pool), setColourBasedOnStrength);
		break;
	}
	}
}