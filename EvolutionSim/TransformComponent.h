#pragma once
#include <glm\ext\vector_float2.hpp>

class TransformComponent
{
	friend class TransformSystem;
	friend struct Being;
	friend struct Camera;
	friend struct BackgroundImage;
	friend struct SelectionBox;

	glm::vec2 pos{ 0.0f };
	glm::vec2 dims{ 0.0f };
};