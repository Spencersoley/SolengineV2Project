#pragma once
#include <cstdint>
#include <glm\ext\vector_float2.hpp>
#include "DefaultColours.h"
#include "SurivalComponent.h"
#include "GeneComponent.h"

struct BeingCreateInfo
{
	uint32_t beingHandle{ 0 };
	glm::vec2 pos{ 0.0f, 0.0f };
	glm::vec2 dims{ 0.0f, 0.0f };
	Colour colour{ 0, 0, 0, 255 };
	SurvivalState survivalState = SurvivalState::AWAITING;
	bool isAlive{ true };
	float fullness{ 0.0f };
	GeneComponent genes;
	float stamina{ 0.0f };
};