#pragma once

#include <SpriteBatch.h>
#include "SpriteComponent.h"

class BeingManager;
class CameraSystem;
class TransformSystem;
namespace SolengineV2
{
	class ShaderManager;
	struct ShaderProgram;
}
using Colour = SolengineV2::Colour;

class SpriteSystem
{
public:
	SpriteSystem(
		const SolengineV2::ShaderManager& shaderManager,
		const SolengineV2::ShaderProgram& defaultShader,
		CameraSystem& camSys,
		const TransformSystem& tformSys,
		const uint32_t backgroundTextureID
	);

	SolengineV2::Colour getColour(const SpriteComponent& component) const { return component.colour; }
	void setColour(SpriteComponent& component, const Colour& col) const { component.colour = col; }

	void process(BeingManager& beings);

private:
	SolengineV2::SpriteBatch spriteBatch;
	const SolengineV2::ShaderManager& shaderManager;
	const SolengineV2::ShaderProgram& shaderProgram;

	CameraSystem& cameraSystem;
	const TransformSystem& transformSystem;

	struct
	{
		SpriteComponent background{ 0, SolengineV2::Colour(255, 255, 255, 255) };
	} components;

	uint32_t getTextureID(const SpriteComponent& component) const { return component.textureID; }

	void renderEntities(SolengineV2::SpriteBatch& batch, const BeingManager& beings) const;

	void render(SolengineV2::SpriteBatch& batch, const glm::vec2& pos, const glm::vec2& dims, const GLuint& textureID, const Colour& col) const;
};
