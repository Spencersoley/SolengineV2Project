#include "GeneSystem.h"

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

const GeneComponent GeneSystem::mutate(const GeneComponent& oldGeneComponent) const
{
	float r = -MAX_SPEED_DECREASE + ((float)rand() / (float)RAND_MAX) * (MAX_SPEED_INCREASE + MAX_SPEED_DECREASE);
	float speed = getTrait(oldGeneComponent, Trait::SPEED) + r;
	speed = std::max(speed, MIN_SPEED);
	speed = std::min(speed, MAX_SPEED);

	r = -MAX_DIET_DECREASE + ((float)rand() / (float)RAND_MAX) * (MAX_DIET_INCREASE + MAX_DIET_DECREASE);
	float diet = getTrait(oldGeneComponent, Trait::DIET) + r;
	diet = std::max(diet, MIN_DIET);
	diet = std::min(diet, MAX_DIET);

	r = -MAX_STRENGTH_DECREASE + ((float)rand() / (float)RAND_MAX) * (MAX_STRENGTH_INCREASE + MAX_STRENGTH_DECREASE);
	float strength = getTrait(oldGeneComponent, Trait::STRENGTH) + r;
	strength = std::max(strength, MIN_STRENGTH);
	strength = std::min(strength, MAX_STRENGTH);

	r = -MAX_STAMINA_DECREASE + ((float)rand() / (float)RAND_MAX) * (MAX_STAMINA_INCREASE + MAX_STAMINA_DECREASE);
	float stamina = getTrait(oldGeneComponent, Trait::STAMINA) + r;
	stamina = std::max(stamina, MIN_STAMINA);
	stamina = std::min(stamina, MAX_STAMINA);


	float hunger = (strength + diet + speed + stamina) / 4;

	return GeneComponent(oldGeneComponent.beingType, speed, diet, strength, stamina, hunger);
}
