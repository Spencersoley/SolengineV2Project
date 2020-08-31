#include <ShaderProgram.h>
#include <ShaderManager.h>
#include "GameData.h"
#include "SpriteSystemSpecialization.h"
#include "CameraSystemSpecialization.h"
#include "TransformSystemSpecialization.h"

void SpriteSystem::update(SolengineV2::SpriteBatch& spriteBatch, SolengineV2::ShaderProgram& shaderProgram, GameData& gameData) const
{
	Camera::System::updateCameraMatrices(gameData);
	SolengineV2::ShaderManager::use(shaderProgram, "mySampler"); //mySampler is name of texture sampler variable in shader
	SolengineV2::ShaderManager::setProjectionMatrix(shaderProgram, Camera::System::getProjectionMatrix(gameData.camera.cam));
	renderEntities(spriteBatch, gameData);
	SolengineV2::ShaderManager::unuse(shaderProgram);
}

void SpriteSystem::renderEntities(SolengineV2::SpriteBatch& spriteBatch, const GameData& gameData) const
{
	auto arenaDiameter = gameData.configurableSettings.arenaDiameter.get();
	spriteBatch.Begin();
	render(
		spriteBatch, 
		Transform::System::getPos(gameData.backgroundImage.transform), 
		glm::vec2(arenaDiameter, arenaDiameter),
		Sprite::System::getTextureID(gameData.backgroundImage.sprite), 
		Sprite::System::getColour(gameData.backgroundImage.sprite));

	const auto renderFood = [this, &spriteBatch](const FoodEntity& food)
	{
		render(spriteBatch, Transform::System::getPos(food.transform), Transform::System::getDims(food.transform), Sprite::System::getTextureID(food.sprite), Sprite::System::getColour(food.sprite));
	};

	std::for_each(cbegin(gameData.foodManager.pool), cend(gameData.foodManager.pool), renderFood);

	const auto renderAnimal = [this, &spriteBatch](const BeingEntity& being)
	{
		render(spriteBatch, Transform::System::getPos(being.transform), Transform::System::getDims(being.transform), Sprite::System::getTextureID(being.sprite), Sprite::System::getColour(being.sprite));
	};

	std::for_each(cbegin(gameData.beingManager.pool), cend(gameData.beingManager.pool), renderAnimal);
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