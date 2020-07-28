#include "TextureSystem.h"
#include <ImageLoader.h>

void TextureSystem::init(SolengineV2::IOManager& ioManager)
{
	SolengineV2::ImageLoader imageLoader(ioManager);
	imageLoader.loadPNG(getTexture(Component::BACKGROUND).data, "Textures/background.png");
	imageLoader.loadPNG(getTexture(Component::BEING_1).data, "Textures/being_1.png");
	imageLoader.loadPNG(getTexture(Component::BEING_2).data, "Textures/being_2.png");
	imageLoader.loadPNG(getTexture(Component::BEING_3).data, "Textures/being_3.png");
	imageLoader.loadPNG(getTexture(Component::BEING_4).data, "Textures/being_4.png");
	imageLoader.loadPNG(getTexture(Component::BEING_5).data, "Textures/being_5.png");
	imageLoader.loadPNG(getTexture(Component::BEING_6).data, "Textures/being_6.png");
	imageLoader.loadPNG(getTexture(Component::BEING_7).data, "Textures/being_7.png");
	imageLoader.loadPNG(getTexture(Component::BEING_8).data, "Textures/being_8.png");
	imageLoader.loadPNG(getTexture(Component::PLANT).data, "Textures/plant.png");
	imageLoader.loadPNG(getTexture(Component::MEAT).data, "Textures/meat.png");
	imageLoader.loadPNG(getTexture(Component::KEYS).data, "Textures/keys.png");
	imageLoader.loadPNG(getTexture(Component::KEYS_2).data, "Textures/keys2.png");
	imageLoader.loadPNG(getTexture(Component::CAMERA).data, "Textures/camera.png");
	imageLoader.loadPNG(getTexture(Component::ZOOM).data, "Textures/zoom.png");
	imageLoader.loadPNG(getTexture(Component::STRENGTH_GRADIENT).data, "Textures/gradient_strength.png");
	imageLoader.loadPNG(getTexture(Component::SPEED_GRADIENT).data, "Textures/gradient_speed.png");
	imageLoader.loadPNG(getTexture(Component::STAMINA_GRADIENT).data, "Textures/gradient_stamina.png");
	imageLoader.loadPNG(getTexture(Component::DIET_GRADIENT).data, "Textures/gradient_diet.png");
}

GLuint TextureSystem::getTextureID(Component component) const
{
	return components[static_cast<size_t>(component)].data.ID;
}


TextureComponent& TextureSystem::getTexture(Component comp)
{
	return components[static_cast<size_t>(comp)];
}
