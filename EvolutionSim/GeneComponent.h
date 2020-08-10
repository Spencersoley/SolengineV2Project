#pragma once
#include <array>
#include "GeneEnum.h"

class GeneComponent
{
	template <class T> friend class GeneSystemInterface;
	Gene::BeingType beingType{ Gene::BeingType::PLANT };
	std::array<float, static_cast<int>(Gene::Trait::TRAIT_COUNT)> traits{ 0.5f, 0.5f, 0.5f, 0.5f, 0.5f };
};