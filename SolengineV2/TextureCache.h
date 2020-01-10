#pragma once
#include <iostream>
#include <map>

#include "ImageLoader.h"
#include "Texture.h"

//This will cache textures, so multiple sprites can use the same texture
namespace SolengineV2
{
	class TextureCache
	{
	public:
		TextureCache(IOManager* IO) : imageLoader(IO) {};
		~TextureCache() {};

		Texture GetTexture(std::string texturePath) 
		{
			//Look up the texture in the map
			auto textureKV = textureMap.find(texturePath);

			//If the texture is found in the map
			if (textureKV == textureMap.end())
			{
				//Load the texture
				Texture newTexture = imageLoader.LoadPNG(texturePath);

				//Insert it into the map
				textureMap.insert(std::make_pair(texturePath, newTexture));
				////textureMap[texturePath] = newTexture;??

				return newTexture;
			}

			//If the texture is found in the map
			return textureKV->second;
		}

	private:
		ImageLoader imageLoader;
		std::map<std::string, Texture> textureMap;
	};
}