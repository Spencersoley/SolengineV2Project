#pragma once
#include <glm\ext\vector_float2.hpp>

#include <Vertex.h>
#include "BeingCreateInfo.h"

using Colour = SolengineV2::Colour;

class GeneSystem
{
	using BeingType = GeneComponent::BeingType;
	using Trait = GeneComponent::Trait;

public:
	BeingType getBeingType(const GeneComponent& gene) const;
	void setBeingType(GeneComponent& gene, BeingType beingType) const;
	float getTrait(const GeneComponent& gene, Trait trait) const;
	void setTrait(GeneComponent& gene, Trait trait, float set) const;

	void mutate(GeneComponent& gene) const;
};