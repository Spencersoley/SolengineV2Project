#pragma once
#include <Texture.h>

class TextureComponent
{
	template <class T> friend class TextureLoaderSystemInterface;
	SolengineV2::Texture data;
};