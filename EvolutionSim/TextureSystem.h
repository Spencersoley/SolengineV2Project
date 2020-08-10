#pragma once
#include <array>
#include "TextureComponent.h"
#include "TextureLibrary.h"

namespace SolengineV2
{
	class IOManager;
}

class TextureSystem
{
public:
	static void loadTextures(SolengineV2::IOManager& ioManager, TextureLibrary& textureLibrary);
};