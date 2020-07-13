#pragma once
#include <cstdint>
#include <Vertex.h>

class SpriteComponent
{
public:
	SpriteComponent() {}
	SpriteComponent(
		const uint32_t _textureID,
		const SolengineV2::Colour& _colour
	) :
		textureID(_textureID),
		colour(_colour)
	{}

private:
	friend class SpriteSystem;
	SolengineV2::Colour colour;
	GLuint textureID{ 0 };
	bool redraw{ true };
	bool IsVisible{ true };
};