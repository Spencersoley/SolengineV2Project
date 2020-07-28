#pragma once
#include <cstdint>
#include <Vertex.h>

class SpriteComponent
{
	friend class SpriteSystem;
	friend struct Being;

	SolengineV2::Colour colour;
	GLuint textureID{ 0 };
	bool redraw{ true };
	bool IsVisible{ true };
};