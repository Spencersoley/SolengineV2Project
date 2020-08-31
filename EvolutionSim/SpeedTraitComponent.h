#pragma once
#include <glm\ext\vector_float2.hpp>

class SpeedTraitComponent
{
	template<class T> friend class SpeedTraitSystemInterface;
	glm::vec2 direction{};
	float value{ 0.5f };
	bool isMoving{ true };
};