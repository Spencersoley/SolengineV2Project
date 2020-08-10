#pragma once

#include "GeneEnum.h"
#include "GeneComponent.h"

template <class Implementation>
class GeneSystemInterface
{
public:
	static float getTrait(const GeneComponent& component, Gene::Trait trait) { return component.traits[static_cast<int>(trait)]; }
	static void setTrait(GeneComponent& component, Gene::Trait trait, const float set) { component.traits[static_cast<int>(trait)] = set; }

	static Gene::BeingType getBeingType(const GeneComponent& component) { return component.beingType; }
	static void setBeingType(GeneComponent& component, Gene::BeingType set) { component.beingType = set; }


	static void mutate(GeneComponent& gene)
	{
		static Implementation system;
		system.mutate(gene);
	}
};


