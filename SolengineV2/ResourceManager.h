#pragma once
#include <string>

#include "TextureCache.h"
#include "Texture.h"

namespace SolengineV2
{
    class ResourceManager
    {
    public:
        ResourceManager(IOManager* io) : textureCache(io) {}
        ~ResourceManager() {}

        Texture GetTexture(std::string texturePath)
        {
            return textureCache.GetTexture(texturePath);
        }

    private:
        TextureCache textureCache;
    };
}