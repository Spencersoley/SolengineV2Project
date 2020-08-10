#include "BeingManager.h"
#include "OverlayConfig.h"
#include "GeneEnum.h"
#include "DefaultColours.h"

#include "OverlaySystemImplementation.h"
#include "SpriteSystemImplementation.h"
#include "SurvivalSystemImplementation.h"
#include "GeneSystemImplementation.h"

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
			static_cast<GLubyte>(static_cast<float>(a.r + (b.r - a.r)) * traitValue0to1 * 2.0f),
			static_cast<GLubyte>(static_cast<float>(a.g + (b.g - a.g)) * traitValue0to1 * 2.0f),
			static_cast<GLubyte>(static_cast<float>(a.b + (b.b - a.b)) * traitValue0to1 * 2.0f),
			255
		);
	}
	else
	{
		return Colour
		(
			static_cast<GLubyte>(static_cast<float>(b.r + (c.r - b.r)) * (traitValue0to1 - 0.5f) * 2.0f),
			static_cast<GLubyte>(static_cast<float>(b.g + (c.g - b.g)) * (traitValue0to1 - 0.5f) * 2.0f),
			static_cast<GLubyte>(static_cast<float>(b.b + (c.b - b.b)) * (traitValue0to1 - 0.5f) * 2.0f),
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

void OverlaySystem::updateOverlay(BeingManager& beings, OverlayConfig& overlay) const
{
	OverlayMode overlayMode = Overlay::System::getOverlay(overlay.component);
	if (overlayMode == OverlayMode::STRENGTH)
	{
		const auto setColourBasedOnStrength = [](Being& being)
		{
			if (Gene::System::getBeingType(being.gene) == Gene::BeingType::ANIMAL && Survival::System::getIsAlive(being.survival))
			{
				float strengthTrait = Gene::System::getTrait(being.gene, Gene::Trait::STRENGTH);

				Sprite::System::setColour(being.sprite, biLerpRGB(LIGHT_YELLOW, RED, strengthTrait));
			}
		};

		std::for_each(begin(beings.pool), end(beings.pool), setColourBasedOnStrength);
	}
	else if (overlayMode == OverlayMode::STAMINA)
	{
		const auto setColourBasedOnStamina = [](Being& being)
		{
			if (Gene::System::getBeingType(being.gene) == Gene::BeingType::ANIMAL && Survival::System::getIsAlive(being.survival))
			{
				float stamina = Gene::System::getTrait(being.gene, Gene::Trait::STAMINA);

				Sprite::System::setColour(being.sprite, biLerpRGB(PURPLE, YELLOW, stamina));
			}
		};

		std::for_each(begin(beings.pool), end(beings.pool), setColourBasedOnStamina);

	}
	else if (overlayMode == OverlayMode::SPEED)
	{
		const auto setColourBasedOnSpeed = [](Being& being)
		{
			if (Gene::System::getBeingType(being.gene) == Gene::BeingType::ANIMAL && Survival::System::getIsAlive(being.survival))
			{
				float speed = Gene::System::getTrait(being.gene, Gene::Trait::SPEED);

				Sprite::System::setColour(being.sprite, biLerpRGB(LIGHT_GREENBLUE, BLUE, speed));
			}
		};

		std::for_each(begin(beings.pool), end(beings.pool), setColourBasedOnSpeed);
	}
	else if (overlayMode == OverlayMode::DIET)
	{
		const auto setColourBasedOnDiet = [](Being& being)
		{
			if (Gene::System::getBeingType(being.gene) == Gene::BeingType::ANIMAL && Survival::System::getIsAlive(being.survival))
			{
				float diet = Gene::System::getTrait(being.gene, Gene::Trait::DIET);

				Sprite::System::setColour(being.sprite, biLerpRGB(DEEP_GREEN, RED, diet));
			}
		};

		std::for_each(begin(beings.pool), end(beings.pool), setColourBasedOnDiet);
	}
	else if (overlayMode == OverlayMode::DEFAULT)
	{
		const auto setColourToDefault = [](Being& being)
		{
			if (Survival::System::getIsAlive(being.survival))
			{
				Sprite::System::setColour(being.sprite, ANIMAL_COLOUR);
			}
			else if (Gene::System::getBeingType(being.gene) == Gene::BeingType::ANIMAL)
			{
				Sprite::System::setColour(being.sprite, DEAD_ANIMAL_COLOUR);
			}
			else
			{
				Sprite::System::setColour(being.sprite, PLANT_COLOUR);
			}
		};

		std::for_each(begin(beings.pool), end(beings.pool), setColourToDefault);
	}
}

