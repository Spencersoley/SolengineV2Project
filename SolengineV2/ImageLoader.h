#pragma once
#include <string>

#include "Texture.h"
#include "PicoPNG.h"
#include "IOManager.h"

namespace SolengineV2 
{
    class ImageLoader
    {
	public:
		ImageLoader(IOManager* IO) { iOManager = IO; }
		~ImageLoader() {};

		// Uses IO manager to obtain the file buffer and PicoPNG to decode the buffer
		// Gives the decoded buffer to openGL and sets textureID
		// The texture ID is used to obtain texture from OpenGL
		Texture LoadPNG(std::string filePath)
		{
			Texture texture = {};
			std::vector<unsigned char> bufferedPNG = iOManager->ReadFileToVectorBuffer(filePath);
			std::vector<unsigned char> decodedPNG;
			unsigned long width, height;
		
			//Using picoPNG ~ error code is non zero if decoding failed
			//sets the decoded png, width, and height.
			int errorCode = picoPNG.decodePNG(decodedPNG, width, height, &(bufferedPNG[0]), bufferedPNG.size());
			if (errorCode != 0)
			{
				throw std::invalid_argument("failed to decode png");
				std::cout << "decodePNG failed with error: " + std::to_string(errorCode);
			}

			glGenTextures(1, &(texture.ID));
			glBindTexture(GL_TEXTURE_2D, texture.ID); 
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(decodedPNG[0]));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			texture.width = width;
			texture.height = height;

			return texture;
		}

	private:
		IOManager* iOManager;
		PicoPNG picoPNG;
    };
}
