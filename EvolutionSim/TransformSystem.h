#pragma once
#include <glm\ext\vector_float2.hpp>

class TransformComponent;

class TransformSystem
{
public:
	void translate(TransformComponent& component, const glm::vec2& transVec) const;
};