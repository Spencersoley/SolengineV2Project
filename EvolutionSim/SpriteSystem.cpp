#pragma once
#include <ShaderProgram.h>
#include <ShaderManager.h>
#include "SpriteSystem.h"
#include "TransformSystem.h"
#include "CameraSystem.h"
#include "BeingManager.h"

using Colour = SolengineV2::Colour;

SpriteSystem::SpriteSystem(
	const SolengineV2::ShaderManager& shaderManager,
	const SolengineV2::ShaderProgram& defaultShader,
	CameraSystem& camSys,
	const TransformSystem& tformSys,
	const uint32_t backgroundTextureID
) :
	shaderManager(shaderManager),
	shaderProgram(defaultShader),
	cameraSystem(camSys),
	transformSystem(tformSys)
{
	components.background.textureID = backgroundTextureID;
}

void SpriteSystem::process(BeingManager& beings)
{
	cameraSystem.updateCameraMatrices();
	shaderManager.use(shaderProgram, "mySampler"); //mySampler is name of texture sampler variable in shader
	shaderManager.setProjectionMatrix(shaderProgram, cameraSystem.getProjectionMatrix());
	spriteBatch.Begin();
	renderEntities(spriteBatch, beings);
	spriteBatch.End();
	spriteBatch.RenderSpriteBatch();
	shaderManager.unuse(shaderProgram);
}

void SpriteSystem::renderEntities(SolengineV2::SpriteBatch& batch, const BeingManager& beings) const
{
	render(batch, transformSystem.getBackgroundPos(), transformSystem.getBackgroundDims(), getTextureID(components.background), getColour(components.background));

	// beings
	for (uint32_t beingHandle = 0; beingHandle < beings.getSize(); beingHandle++)
	{
		const TransformComponent& transform = beings.getTransformComponent(beingHandle);
		const SpriteComponent& sprite = beings.getSpriteComponent(beingHandle);
		render(batch, transformSystem.getPos(transform), transformSystem.getDims(transform), getTextureID(sprite), getColour(sprite));
	}
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

//frustum culling
//bool isInView(TransformComponent* obj, TransformComponent* cam, int sw, int sh, float scale) const
//{
//	glm::vec3 pos = transformSystem->getPos(obj);
//	glm::vec3 dims = transformSystem->getDims(obj);

//	glm::vec2 scaledScreenDims = glm::vec2(sw, sh) / scale;

//	const float MIN_DISTANCE_X = dims.x / 2.0f + scaledScreenDims.x / 2.0f;
//	const float MIN_DISTANCE_Y = dims.y / 2.0f + scaledScreenDims.y / 2.0f;

//	//Get a vector of the distance between the colliding tile and the agent
//	glm::vec2 centrePos = glm::vec2(pos.x, pos.y);
//	glm::vec2 distVec = centrePos - glm::vec2(pos.x, pos.y);

//	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
//	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

//	// If the minimum collision distance is greater than the current distance, we have a collision
//	if (xDepth > 0 && yDepth > 0)
//	{
//		return true;
//	}

//	return false;
//}
