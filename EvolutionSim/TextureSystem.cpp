#include <ImageLoader.h>
#include "TextureLoaderSystemImplementation.h"

void TextureSystem::loadTextures(SolengineV2::IOManager& ioManager, TextureLibrary& textureLibrary)
{
	SolengineV2::ImageLoader imageLoader(ioManager);
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::BACKGROUND, textureLibrary), "Textures/background.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::BEING_1, textureLibrary), "Textures/being_1.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::BEING_2, textureLibrary), "Textures/being_2.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::BEING_3, textureLibrary), "Textures/being_3.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::BEING_4, textureLibrary), "Textures/being_4.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::BEING_5, textureLibrary), "Textures/being_5.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::BEING_6, textureLibrary), "Textures/being_6.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::BEING_7, textureLibrary), "Textures/being_7.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::BEING_8, textureLibrary), "Textures/being_8.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::PLANT, textureLibrary), "Textures/plant.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::MEAT, textureLibrary), "Textures/meat.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::KEYS, textureLibrary), "Textures/keys.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::KEYS_2, textureLibrary), "Textures/keys2.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::CAMERA, textureLibrary), "Textures/camera.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::ZOOM, textureLibrary), "Textures/zoom.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::STRENGTH_GRADIENT, textureLibrary), "Textures/gradient_strength.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::SPEED_GRADIENT, textureLibrary), "Textures/gradient_speed.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::STAMINA_GRADIENT, textureLibrary), "Textures/gradient_stamina.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibrary::Texture ::DIET_GRADIENT, textureLibrary), "Textures/gradient_diet.png");
}

