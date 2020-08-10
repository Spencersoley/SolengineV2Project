#pragma once
#include <glm\ext\vector_float2.hpp>
#include <SpriteBatch.h>

struct CameraManager;
class BeingManager;
struct BackgroundImage;
class SpriteComponent;

namespace SolengineV2
{
	class ShaderProgram;
	class SpriteBatch;
}

class SpriteSystem
{
	using Colour = SolengineV2::Colour;
public:
	void update(BeingManager& beings, BackgroundImage& background, CameraManager& camera, SolengineV2::SpriteBatch& spriteBatch, SolengineV2::ShaderProgram& shaderProgram);

private:
	void renderEntities(const BeingManager& beings, const BackgroundImage& backGround, SolengineV2::SpriteBatch& spriteBatch) const;
	void render(SolengineV2::SpriteBatch& spriteBatch, const glm::vec2& pos, const glm::vec2& dims, const GLuint textureID, const Colour& col) const;
};
