#pragma once
#include "TextureLibrary.h"
#include "TextureComponent.h"

namespace SolengineV2
{
	class IOManager;
}

template <class Implementation>
class TextureLoaderSystemInterface
{
public:
	static GLuint getTextureID(TextureLibrary::Texture component, TextureLibrary& textureLibrary)
	{
		return getTexture(component, textureLibrary).ID;
	}

	static SolengineV2::Texture& getTexture(TextureLibrary::Texture component, TextureLibrary& textureLibrary)
	{
		return textureLibrary.components[static_cast<size_t>(component)].data;
	}

	static void loadTextures(SolengineV2::IOManager& ioManager, TextureLibrary& textureLibrary)
	{
		static Implementation system;
		system.loadTextures(ioManager, textureLibrary);
	}
};