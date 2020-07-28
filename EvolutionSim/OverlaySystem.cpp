#include "BeingManager.h"
#include "GeneSystem.h"
#include "OverlaySystem.h"
#include "SpriteSystem.h"
#include "SurvivalSystem.h"
#include "DefaultColours.h"

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

void OverlaySystem::setOverlay(Overlay overlay)
{
	currentOverlay = overlay;
}

void OverlaySystem::updateOverlay(BeingManager& beings) const
{
	if (currentOverlay == Overlay::STRENGTH)
	{
		const auto setColourBasedOnStrength = [this](Being& being)
		{
			if (this->geneSystem.getBeingType(being.gene) == GeneComponent::BeingType::ANIMAL && this->survivalSystem.getIsAlive(being.survival))
			{
				float strengthTrait = this->geneSystem.getTrait(being.gene, GeneComponent::Trait::STRENGTH);

				spriteSystem.setColour(being.sprite, biLerpRGB(LIGHT_YELLOW, RED, strengthTrait));
			}
		};

		std::for_each(begin(beings.pool), end(beings.pool), setColourBasedOnStrength);
	}
	else if (currentOverlay == Overlay::STAMINA)
	{
		const auto setColourBasedOnStamina = [this](Being& being)
		{
			if (this->geneSystem.getBeingType(being.gene) == GeneComponent::BeingType::ANIMAL && this->survivalSystem.getIsAlive(being.survival))
			{
				float stamina = this->geneSystem.getTrait(being.gene, GeneComponent::Trait::STAMINA);

				spriteSystem.setColour(being.sprite, biLerpRGB(PURPLE, YELLOW, stamina));
			}
		};

		std::for_each(begin(beings.pool), end(beings.pool), setColourBasedOnStamina);

	}
	else if (currentOverlay == Overlay::SPEED)
	{
		const auto setColourBasedOnSpeed = [this](Being& being)
		{
			if (this->geneSystem.getBeingType(being.gene) == GeneComponent::BeingType::ANIMAL && this->survivalSystem.getIsAlive(being.survival))
			{
				float speed = this->geneSystem.getTrait(being.gene, GeneComponent::Trait::SPEED);

				spriteSystem.setColour(being.sprite, biLerpRGB(LIGHT_GREENBLUE, BLUE, speed));
			}
		};

		std::for_each(begin(beings.pool), end(beings.pool), setColourBasedOnSpeed);
	}
	else if (currentOverlay == Overlay::DIET)
	{
		const auto setColourBasedOnDiet = [this](Being& being)
		{
			if (this->geneSystem.getBeingType(being.gene) == GeneComponent::BeingType::ANIMAL && this->survivalSystem.getIsAlive(being.survival))
			{
				float diet = this->geneSystem.getTrait(being.gene, GeneComponent::Trait::DIET);

				spriteSystem.setColour(being.sprite, biLerpRGB(DEEP_GREEN, RED, diet));
			}
		};

		std::for_each(begin(beings.pool), end(beings.pool), setColourBasedOnDiet);
	}
	else if (currentOverlay == Overlay::DEFAULT)
	{
		const auto setColourToDefault = [this](Being& being)
		{
			if (this->survivalSystem.getIsAlive(being.survival))
			{
				spriteSystem.setColour(being.sprite, ANIMAL_COLOUR);
			}
			else if (this->geneSystem.getBeingType(being.gene) == GeneComponent::BeingType::ANIMAL)
			{
				spriteSystem.setColour(being.sprite, DEAD_ANIMAL_COLOUR);
			}
			else
			{
				spriteSystem.setColour(being.sprite, PLANT_COLOUR);
			}
		};

		std::for_each(begin(beings.pool), end(beings.pool), setColourToDefault);
	}
}

OverlaySystem::Overlay OverlaySystem::getOverlay() const
{
	return currentOverlay;
}