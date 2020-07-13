#pragma once
#include <glm\ext\vector_float2.hpp>

class TransformComponent
{
public:
	TransformComponent() {};
	TransformComponent(
		const glm::vec2& pos,
		const glm::vec2& dims
	) :
		pos(pos),
		dims(dims)
	{}

private:
	friend class TransformSystem;
	glm::vec2 pos{ 0.0f };
	glm::vec2 dims{ 0.0f };
};