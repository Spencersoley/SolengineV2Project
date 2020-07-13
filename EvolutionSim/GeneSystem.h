#pragma once
#include <glm\ext\vector_float2.hpp>

#include "GeneComponent.h"

struct DuplicationData
{
	const glm::vec2 pos{};
	const bool isAlive{ false };
	const float currentFullness{ 0.0f };
	const GeneComponent genes{ GeneComponent::BeingType::PLANT, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f };
};

class GeneSystem
{
	using Trait = GeneComponent::Trait;
	using BeingType = GeneComponent::BeingType;

public:
	BeingType getBeingType(const GeneComponent& gene) const { return gene.beingType; }
	float getTrait(const GeneComponent& gene, Trait trait) const { return gene.traits[static_cast<int>(trait)]; }

	const GeneComponent mutate(const GeneComponent& oldGeneComponent) const;
};