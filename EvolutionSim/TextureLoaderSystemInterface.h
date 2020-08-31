#pragma once
#include "TextureLibraryEntity.h"
#include "TextureComponent.h"
#include "GameData.h"
namespace SolengineV2
{
	class IOManager;
}

template <class Implementation>
class TextureLoaderSystemInterface
{
public:
	static GLuint getTextureID(TextureComponent& component) { return component.data.ID; }
	static SolengineV2::Texture& getTexture(TextureComponent& component) { return component.data; }
	
	static GLuint getTextureID(TextureLibraryEntity::Texture component, GameData& gameData)
	{
		static Implementation system;
		return system.getTexture(component, gameData).ID;
	}

	static SolengineV2::Texture& getTexture(TextureLibraryEntity::Texture component, GameData& gameData)
	{
		static Implementation system;
		return system.getTexture(component, gameData);
	}

	static void loadTextures(SolengineV2::IOManager& ioManager, GameData& gameData)
	{
		static Implementation system;
		system.loadTextures(ioManager, gameData);
	}
};