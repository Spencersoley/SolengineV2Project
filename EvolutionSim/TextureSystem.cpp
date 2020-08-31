#include <ImageLoader.h>
#include "TextureLoaderSystemSpecialization.h"
#include "GameData.h"

void TextureSystem::loadTextures(SolengineV2::IOManager& ioManager, GameData& gameData)
{
	SolengineV2::ImageLoader imageLoader(ioManager);
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::BACKGROUND, gameData), "Textures/background.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::BEING_1, gameData), "Textures/being_1.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::BEING_2, gameData), "Textures/being_2.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::BEING_3, gameData), "Textures/being_3.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::BEING_4, gameData), "Textures/being_4.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::BEING_5, gameData), "Textures/being_5.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::BEING_6, gameData), "Textures/being_6.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::BEING_7, gameData), "Textures/being_7.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::BEING_8, gameData), "Textures/being_8.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::PLANT, gameData), "Textures/plant.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::MEAT, gameData), "Textures/meat.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::KEYS, gameData), "Textures/keys.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::KEYS_2, gameData), "Textures/keys2.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::CAMERA, gameData), "Textures/camera.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::ZOOM, gameData), "Textures/zoom.png");
	imageLoader.loadPNG(TextureLoader::System::getTexture(TextureLibraryEntity::Texture ::GRADIENT, gameData), "Textures/gradient.png");
}

SolengineV2::Texture& TextureSystem::getTexture(TextureLibraryEntity::Texture component, GameData& gameData)
{
	return TextureLoader::System::getTexture(gameData.textureLibrary.components[static_cast<int>(component)]);
}