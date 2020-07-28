#pragma once
#include <ShaderProgram.h>
#include <ShaderManager.h>
#include "SpriteSystem.h"
#include "TransformSystem.h"
#include "CameraSystem.h"
#include "BeingManager.h"

#include "SpriteComponent.h"

#include "Camera.h"
#include "BackgroundImage.h"

using Colour = SolengineV2::Colour;

SpriteSystem::SpriteSystem(
	const SolengineV2::ShaderManager& shaderManager,
	const SolengineV2::ShaderProgram& defaultShader,
	const CameraSystem& camSys,
	const TransformSystem& tformSys
) :
	shaderManager(shaderManager),
	shaderProgram(defaultShader),
	cameraSystem(camSys),
	transformSystem(tformSys)
{}

void SpriteSystem::update(BeingManager& beings, BackgroundImage& background, Camera& camera)
{
	cameraSystem.updateCameraMatrices(camera.cam, camera.transform);
	shaderManager.use(shaderProgram, "mySampler"); //mySampler is name of texture sampler variable in shader
	shaderManager.setProjectionMatrix(shaderProgram, cameraSystem.getProjectionMatrix(camera.cam));
	spriteBatch.Begin();
	renderEntities(spriteBatch, beings, background);
	spriteBatch.End();
	spriteBatch.RenderSpriteBatch();
	shaderManager.unuse(shaderProgram);
}

SolengineV2::Colour SpriteSystem::getColour(const SpriteComponent& component) const
{
	return component.colour;
}

void SpriteSystem::setColour(SpriteComponent& component, const Colour& col) const
{
	component.colour = col;
}

void SpriteSystem::setTextureID(SpriteComponent& component, const GLuint id) const
{
	component.textureID = id;
}

unsigned int SpriteSystem::getTextureID(const SpriteComponent& component) const
{
	return component.textureID;
}
void SpriteSystem::renderEntities(SolengineV2::SpriteBatch& batch, const BeingManager& beings, const BackgroundImage& background) const
{
	render(batch, transformSystem.getPos(background.transform), transformSystem.getDims(background.transform), getTextureID(background.sprite), getColour(background.sprite));

	const auto renderBeing = [&batch, this](const Being& being)
	{
		const TransformComponent& transform = being.transform;
		const SpriteComponent& sprite = being.sprite;
		render(batch, transformSystem.getPos(transform), transformSystem.getDims(transform), getTextureID(sprite), getColour(sprite));
	};

	std::for_each(cbegin(beings.pool), cend(beings.pool), renderBeing);
}

//could we just move this into the engine?
void SpriteSystem::render(SolengineV2::SpriteBatch& batch, const glm::vec2& pos, const glm::vec2& dims, const GLuint& textureID, const Colour& col) const
{
	const glm::vec4 destRect
	{
		pos.x - dims.x / 2.0f,
		pos.y - dims.y / 2.0f,
		dims.x,
		dims.y
	};

	batch.Draw(destRect, glm::vec4{ (0.0f, 0.0f, 1.0f, 1.0f) }, textureID, 0.0f, col);
}
