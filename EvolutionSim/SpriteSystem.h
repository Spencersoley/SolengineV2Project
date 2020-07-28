#pragma once
#include <SpriteBatch.h>
#include <glm\ext\vector_float2.hpp>

namespace SolengineV2
{
	class ShaderManager;
	struct ShaderProgram;
}
class BeingManager;
struct Camera;
struct BackgroundImage;
class SpriteComponent;
class CameraSystem;
class TransformSystem;

class SpriteSystem
{
	using Colour = SolengineV2::Colour;
public:
	SpriteSystem(
		const SolengineV2::ShaderManager& shaderManager,
		const SolengineV2::ShaderProgram& defaultShader,
		const CameraSystem& camSys,
		const TransformSystem& tformSys
	);
	
	void update(BeingManager& beings, BackgroundImage& background, Camera& camera);

	SolengineV2::Colour getColour(const SpriteComponent& component) const;
	void setColour(SpriteComponent& component, const Colour& col) const;
	void setTextureID(SpriteComponent& component, const GLuint id) const;

private:
	SolengineV2::SpriteBatch spriteBatch;
	const SolengineV2::ShaderManager& shaderManager;
	const SolengineV2::ShaderProgram& shaderProgram;

	const CameraSystem& cameraSystem;
	const TransformSystem& transformSystem;

	unsigned int getTextureID(const SpriteComponent& component) const;
	void renderEntities(SolengineV2::SpriteBatch& batch, const BeingManager& beings, const BackgroundImage& backGround) const;
	void render(SolengineV2::SpriteBatch& batch, const glm::vec2& pos, const glm::vec2& dims, const GLuint& textureID, const Colour& col) const;
};
