#pragma once
#include <SpriteBatch.h>
#include <ShaderProgram.h>

#include "SpriteComponent.h"

class BackgroundImage;
class CameraManager;
class BeingManager;

template <class Implementation>
class SpriteSystemInterface
{
	using Colour = SolengineV2::Colour;
public:
	static const SolengineV2::Colour& getColour(const SpriteComponent& component) { return component.colour; }
	static void setColour(SpriteComponent& component, const Colour& set) { component.colour = set; }
	static void setTextureID(SpriteComponent& component, const GLuint set) { component.textureID = set; }
	static GLuint getTextureID(const SpriteComponent& component) { return component.textureID; }

	static void update(BeingManager& beings, BackgroundImage& background, CameraManager& camera, SolengineV2::SpriteBatch& spriteBatch, SolengineV2::ShaderProgram& shaderProgram)
	{
		static Implementation system;
		system.update(beings, background, camera, spriteBatch, shaderProgram);
	};
};



