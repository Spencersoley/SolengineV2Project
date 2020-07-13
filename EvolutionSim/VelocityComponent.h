#pragma once
#include <glm\ext\vector_float2.hpp>

class VelocityComponent
{
public:
	VelocityComponent() {}
	VelocityComponent(
		const float vel, 
		const glm::vec2& dir
	) :
		velocity(vel),
		direction(dir)
	{}

private:
	friend class VelocitySystem;
	float velocity{ 0.0f };
	glm::vec2 direction{ 0.0f };
};

