#pragma once
#include <array>
#include "TextureComponent.h"
namespace SolengineV2
{
	class IOManager;
}

class TextureSystem
{
public:
	static void loadTextures(SolengineV2::IOManager& ioManager, GameData& gameData);

	static SolengineV2::Texture& getTexture(TextureLibraryEntity::Texture component, GameData& gameData);
};