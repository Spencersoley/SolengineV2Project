#pragma once

struct GraphicsComponent
{
	GraphicsComponent(SolengineV2::SpriteBatch* SB, std::shared_ptr<TransformComponent> TC, GLuint textID, SolengineV2::Colour col) :
		textureID(textID),
		colour(col),
		redraw(true),
		transformCo(TC),
		spriteBatch(SB)
	{}

	SolengineV2::Colour colour;
	SolengineV2::SpriteBatch* spriteBatch;
	std::shared_ptr<TransformComponent> transformCo;
	GLuint textureID;
	bool redraw;
};
