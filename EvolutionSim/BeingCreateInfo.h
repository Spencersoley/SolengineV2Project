#pragma once
#include <cstdint>
#include <glm\ext\vector_float2.hpp>
#include <Vertex.h>
#include "SurvivalComponent.h"
#include "GeneComponent.h"

using BeingType = GeneComponent::BeingType;
using Colour = SolengineV2::Colour;
//dims, survivalState, pos, isAlive, currentFullness, colour, beingType, diet, hunger, speed, stamina, strength
struct BeingCreateInfo
{
	glm::vec2 dims{ 0.0f, 0.0f };
	SurvivalComponent::SurvivalState survivalState = SurvivalComponent::SurvivalState::AWAITING;
	glm::vec2 pos{};
	bool isAlive{ false };
	float currentFullness{ 0.0f };
	SolengineV2::Colour colour{ 0, 0, 0, 0 };
	BeingType beingType{ BeingType::PLANT };
	float dietTrait{ 0.0f };
	float hungerTrait{ 0.0f };
	float speedTrait{ 0.0f };
	float staminaTrait{ 0.0f };
	float strengthTrait{ 0.0f };
};