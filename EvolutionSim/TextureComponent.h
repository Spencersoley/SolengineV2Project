#pragma once
#include <Texture.h>

class TextureComponent
{
private:
	template <class T> friend class TextureLoaderSystemInterface;
	SolengineV2::Texture data;
};