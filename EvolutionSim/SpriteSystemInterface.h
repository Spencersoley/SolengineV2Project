#pragma once
#include <SpriteBatch.h>
#include <ShaderProgram.h>
#include "SpriteComponent.h"
struct GameData;

template <class Implementation>
class SpriteSystemInterface
{
	using Colour = SolengineV2::Colour;
public:
	static const SolengineV2::Colour& getColour(const SpriteComponent& component) { return component.colour; }
	static void setColour(SpriteComponent& component, const Colour& set) { component.colour = set; }
	static void setTextureID(SpriteComponent& component, const GLuint set) { component.textureID = set; }
	static GLuint getTextureID(const SpriteComponent& component) { return component.textureID; }

	static void update(SolengineV2::SpriteBatch& spriteBatch, SolengineV2::ShaderProgram& shaderProgram, GameData& gameData)
	{
		static Implementation system;
		system.update(spriteBatch, shaderProgram, gameData);
	};
};