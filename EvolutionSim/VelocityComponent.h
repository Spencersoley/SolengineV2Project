#pragma once
#include <glm\ext\vector_float2.hpp>

class VelocityComponent
{
	friend class VelocitySystem;
	friend class BeingManager;

	glm::vec2 direction{ 0.0f };
	float velocity{ 0.0f };
};

