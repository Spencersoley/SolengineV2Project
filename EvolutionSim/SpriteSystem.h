#pragma once
#include <glm\ext\vector_float2.hpp>
#include <SpriteBatch.h>
struct GameData;
namespace SolengineV2
{
	struct ShaderProgram;
	class SpriteBatch;
}

class SpriteSystem
{
	using Colour = SolengineV2::Colour;
public:
	void update(SolengineV2::SpriteBatch& spriteBatch, SolengineV2::ShaderProgram& shaderProgram, GameData& gameData) const;

private:
	void renderEntities(SolengineV2::SpriteBatch& spriteBatch, const GameData& gameData) const;
	void render(SolengineV2::SpriteBatch& spriteBatch, const glm::vec2& pos, const glm::vec2& dims, const GLuint textureID, const Colour& col) const;
};