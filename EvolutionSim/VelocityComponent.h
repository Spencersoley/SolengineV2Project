#pragma once
#include <glm\ext\vector_float2.hpp>

class VelocityComponent
{
	template<class T> friend class VelocitySystemInterface;

	glm::vec2 direction{ 0.0f };
	float velocity{ 0.0f };
};

