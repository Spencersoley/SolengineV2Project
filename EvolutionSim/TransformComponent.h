#pragma once
#include <glm\ext\vector_float2.hpp>

class TransformComponent
{
	template <class T> friend class TransformSystemInterface;

	glm::vec2 pos{ 0.0f };
	glm::vec2 dims{ 0.0f };
};