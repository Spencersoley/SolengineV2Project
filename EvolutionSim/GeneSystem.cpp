#include <Vertex.h>

#include "GeneEnum.h"

#include "GeneSystemImplementation.h"

constexpr float MIN_SPEED = 0.02f;
constexpr float MAX_SPEED = 1.0f;
constexpr float MAX_SPEED_DECREASE = 0.02f;
constexpr float MAX_SPEED_INCREASE = 0.02f;

constexpr float MIN_DIET = 0.0f;
constexpr float MAX_DIET = 1.0f;
constexpr float MAX_DIET_DECREASE = 0.02f;
constexpr float MAX_DIET_INCREASE = 0.02f;

constexpr float MIN_STRENGTH = 0.2f;
constexpr float MAX_STRENGTH = 1.0f;
constexpr float MAX_STRENGTH_DECREASE = 0.02f;
constexpr float MAX_STRENGTH_INCREASE = 0.02f;

constexpr float MIN_STAMINA = 0.2f;
constexpr float MAX_STAMINA = 1.0f;
constexpr float MAX_STAMINA_DECREASE = 0.02f;
constexpr float MAX_STAMINA_INCREASE = 0.02f;

using BeingType = Gene::BeingType;
using Colour = SolengineV2::Colour;
using Trait = Gene::Trait;

void GeneSystem::mutate(GeneComponent& gene) const
{
	float r = -MAX_SPEED_DECREASE + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (MAX_SPEED_INCREASE + MAX_SPEED_DECREASE);
	float speed = Gene::System::getTrait(gene, Trait::SPEED) + r;
	speed = std::max(speed, MIN_SPEED);
	speed = std::min(speed, MAX_SPEED);
	Gene::System::setTrait(gene, Trait::SPEED, speed);

	r = -MAX_DIET_DECREASE + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (MAX_DIET_INCREASE + MAX_DIET_DECREASE);
	float diet = Gene::System::getTrait(gene, Trait::DIET) + r;
	diet = std::max(diet, MIN_DIET);
	diet = std::min(diet, MAX_DIET);
	Gene::System::setTrait(gene, Trait::DIET, diet);

	r = -MAX_STRENGTH_DECREASE + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (MAX_STRENGTH_INCREASE + MAX_STRENGTH_DECREASE);
	float strength = Gene::System::getTrait(gene, Trait::STRENGTH) + r;
	strength = std::max(strength, MIN_STRENGTH);
	strength = std::min(strength, MAX_STRENGTH);
	Gene::System::setTrait(gene, Trait::STRENGTH, strength);

	r = -MAX_STAMINA_DECREASE + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (MAX_STAMINA_INCREASE + MAX_STAMINA_DECREASE);
	float stamina = Gene::System::getTrait(gene, Trait::STAMINA) + r;
	stamina = std::max(stamina, MIN_STAMINA);
	stamina = std::min(stamina, MAX_STAMINA);
	Gene::System::setTrait(gene, Trait::STAMINA, stamina);

	float hunger = (strength + diet + speed + stamina) / 4;
	Gene::System::setTrait(gene, Trait::HUNGER, hunger);
}