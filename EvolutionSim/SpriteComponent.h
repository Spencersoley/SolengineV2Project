#pragma once
#include <cstdint>
#include <Vertex.h>

class SpriteComponent
{
	template <class T> friend class SpriteSystemInterface;

	SolengineV2::Colour colour;
	GLuint textureID{ 0 };
	bool redraw{ true };
	bool IsVisible{ true };
};