#pragma once
#include "TextureComponent.h"
#include <array>

namespace SolengineV2
{
	class IOManager;
}

struct TextureSystem
{
	void init(SolengineV2::IOManager& ioManager);

	enum class Component
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
		STRENGTH_GRADIENT,
		STAMINA_GRADIENT,
		SPEED_GRADIENT,
		DIET_GRADIENT,
		N_OF_COMPONENTS
	};

	GLuint getTextureID(Component component) const;

private:
	std::array <TextureComponent, static_cast<size_t>(Component::N_OF_COMPONENTS)> components;

	TextureComponent& getTexture(Component comp);
};