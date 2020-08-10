#include <ShaderProgram.h>
#include <ShaderManager.h>

#include "CameraManager.h"
#include "BackgroundImage.h"
#include "BeingManager.h"

#include "SpriteSystemImplementation.h"
#include "CameraSystemImplementation.h"
#include "TransformSystemImplementation.h"

void SpriteSystem::update(BeingManager& beings, BackgroundImage& background, CameraManager& camera, SolengineV2::SpriteBatch& spriteBatch, SolengineV2::ShaderProgram& shaderProgram)
{
	Camera::System::updateCameraMatrices(camera.cam, camera.transform);
	SolengineV2::ShaderManager::use(shaderProgram, "mySampler"); //mySampler is name of texture sampler variable in shader
	SolengineV2::ShaderManager::setProjectionMatrix(shaderProgram, Camera::System::getProjectionMatrix(camera.cam));
	renderEntities(beings, background, spriteBatch);
	SolengineV2::ShaderManager::unuse(shaderProgram);
}

void SpriteSystem::renderEntities(const BeingManager& beings, const BackgroundImage& background, SolengineV2::SpriteBatch& spriteBatch) const
{
	spriteBatch.Begin();
	render(spriteBatch, Transform::System::getPos(background.transform), Transform::System::getDims(background.transform), Sprite::System::getTextureID(background.sprite), Sprite::System::getColour(background.sprite));

	const auto renderBeing = [this, &spriteBatch](const Being& being)
	{
		render(spriteBatch, Transform::System::getPos(being.transform), Transform::System::getDims(being.transform), Sprite::System::getTextureID(being.sprite), Sprite::System::getColour(being.sprite));
	};

	std::for_each(cbegin(beings.pool), cend(beings.pool), renderBeing);
	spriteBatch.End();
	spriteBatch.RenderSpriteBatch();
}

void SpriteSystem::render(SolengineV2::SpriteBatch& spriteBatch, const glm::vec2& pos, const glm::vec2& dims, const GLuint textureID, const Colour& col) const
{
	const glm::vec4 destRect
	{
		pos.x - dims.x / 2.0f,
		pos.y - dims.y / 2.0f,
		dims.x,
		dims.y
	};

	spriteBatch.Draw(destRect, glm::vec4{ (0.0f, 0.0f, 1.0f, 1.0f) }, textureID, 0.0f, col);
}
