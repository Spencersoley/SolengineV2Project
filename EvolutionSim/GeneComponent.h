#pragma once
#include "AggressionTraitComponent.h"
#include "DietTraitComponent.h"
#include "HealthTraitComponent.h"
#include "HungerTraitComponent.h"
#include "IntelligenceTraitComponent.h"
#include "SpeedTraitComponent.h"
#include "StaminaTraitComponent.h"
#include "StrengthTraitcomponent.h"
#include "TriQueue.h"

class GeneComponent
{
	template <class T> friend class GeneSystemInterface;
	// We could pack these better non-alphabetically
	AggressionTraitComponent aggression;
	DietTraitComponent diet;
	HealthTraitComponent health;
	HungerTraitComponent hunger;
	IntelligenceTraitComponent intelligence;
	SpeedTraitComponent speed;
	StaminaTraitComponent stamina;
	StrengthTraitComponent strength;
	TriQueue<unsigned int> familyHistory;
};