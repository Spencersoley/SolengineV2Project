#pragma once
#include <array>
#include "TextureComponent.h"

class TextureLibraryEntity
{
public:
	enum class Texture
	{
		BEING_1,
		BEING_2,
		BEING_3,
		BEING_4,
		BEING_5,
		BEING_6,
		BEING_7,
		BEING_8,
		BACKGROUND,
		PLANT,
		MEAT,
		KEYS,
		KEYS_2,
		CAMERA,
		ZOOM,
		GRADIENT,
		N_OF_COMPONENTS
	};

	std::array<TextureComponent, static_cast<size_t>(Texture::N_OF_COMPONENTS)> components;
};