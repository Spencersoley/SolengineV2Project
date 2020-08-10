#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <set>
#include <IMGUI\imgui.h>

#include <ImageLoader.h>
#include <ShaderCreator.h>
#include <ShaderManager.h>
#include <Window.h>
#include <TimeManager.h>
#include <SDLInit.h>
#include <GameState.h>

#include "ArenaSize.h"
#include "BeingManager.h"
#include "DataPointManager.h"
#include "BackgroundImage.h"
#include "CameraManager.h"
#include "SelectionBox.h"
#include "GenerationManager.h"
#include "OverlayConfig.h"
#include "SelectedTracker.h"
#include "Sceensize.h"
#include "SpatialPartition.h"
#include "TextureLibrary.h"

#include "CameraSystemImplementation.h"
#include "ColliderSystemImplementation.h"
#include "DataSystemImplementation.h"
#include "GenerationSystemImplementation.h"
#include "GUISystemImplementation.h"
#include "PartitionSystemImplementation.h"
#include "SelectableSystemImplementation.h"
#include "ShapeSystemImplementation.h"
#include "TransformSystemImplementation.h"
#include "SpriteSystemImplementation.h"
#include "SurvivalSystemImplementation.h"
#include "UserInputSystemImplementation.h"	
#include "VelocitySystemImplementation.h"

#include "OverlaySystemInterface.h"	
#include "TextureLoaderSystemImplementation.h"

/////////////////// TODO: FEATURES //////////////////

// high aggression will mean entities will seek out other entities to kill over heading for food
// higher strength means that aggressive entities will eat other entities
// more passive beings will retreat from aggressive beings - intelligence will be a factor in this
// aggression over 6 causes animals to attack animals that it comes into contact with
// aggression over 8 causes animals to seek animals over food

// intelligence provides big bonuses to defence against agressor,
// intelligence provides bonuses to passivity avoidance, 
// intelligence allows non aggressive carnivores to prioritise alive weaker animals if they want

// toughness corresponds with animal health - high toughness allows beings to survive attacks

// MEAT gui more detailed/different to animal

// more variety in plant strength/size

// variety in animal strength/size?

//   the right food types provide huge bonuses 
// - selected entity plant saturation/fullness
// - choose active traits at start of simulation?
//   make optional all traits through a configurationSystem?

// Colour gradient and general colour trait coordination/association
// designated overlay menu

//////////////////  TODO: STYLE /////////////////
// - what int type to use throughout the projec?
// - change main into application?
// - neated guisystem
// - textures --> texture system

///////////////// TODO: OPTIMISATION ///////////////
// - figure out a system to squash data 

int main(int argc, char** argv)
{
	SolengineV2::initialiseSDL();

	struct Shaders
	{
		SolengineV2::ShaderProgram colourShading;
		SolengineV2::ShaderProgram simpleGeometry;
	} shaders;

	SolengineV2::Window            window("SolengineV2", SCREEN_WIDTH, SCREEN_HEIGHT, SolengineV2::Colour(0, 0, 0, 255));
	SolengineV2::ShaderManager     shaderManager;
	SolengineV2::TimeManager       timeManager(100000, SolengineV2::TimerMode::LIFETIME_AVERAGE);
	SolengineV2::InputManager      inputManager;

	SolengineV2::SpriteBatch       spriteBatch;

	TextureLibrary textureLibrary;

	{
		SolengineV2::IOManager         ioManager;
		SolengineV2::ShaderCreator     shaderCreator(ioManager);
		shaderCreator.createShader(shaders.colourShading, "Shaders/colourShading.vert", "Shaders/colourShading.frag", { "vertexPosition", "vertexColour", "vertexUV" });
		shaderCreator.createShader(shaders.simpleGeometry, "Shaders/simpleGeometry.vert", "Shaders/simpleGeometry.frag", { "aPos" });
		TextureLoader::System::loadTextures(ioManager, textureLibrary);
	}

	GUI::System::init(window);

	SolengineV2::GameState gameState{ SolengineV2::GameState::PLAY };

	//Entities
	BeingManager      beingManager;
	DataPointManager  dataPointManager;
	GenerationManager generationManager;

	OverlayConfig     overlayConfig;

	const int INITIAL_FOOD_COUNT = 25;
	Generation::System::setFoodPerGeneration(generationManager.component, INITIAL_FOOD_COUNT);

	SelectionBox     selectionBox;
	Transform::System::setPos(selectionBox.transform, glm::vec2(FLT_MAX));
	Transform::System::setDims(selectionBox.transform, glm::vec2(25.0f));

	CameraManager           camera;
	Transform::System::setPos(camera.transform, { -300.0f, 0.0f });
	Transform::System::setDims(camera.transform, { 0.0f, 0.0f });

	BackgroundImage  backgroundImage;
	Transform::System::setPos(backgroundImage.transform, { 0.0f, 0.0f });
	Transform::System::setDims(backgroundImage.transform, { ARENA_SIZE, ARENA_SIZE });
	Sprite::System::setColour(backgroundImage.sprite, { 255, 255, 255, 255 });
	Sprite::System::setTextureID(backgroundImage.sprite, TextureLoader::System::getTextureID(TextureLibrary::Texture::BACKGROUND, textureLibrary));

	SelectedTracker selectedTracker;

	SpatialPartition spatialPartition;
	Partition::System::setupGrid(spatialPartition.partition);

	ImGuiIO io{ ImGui::GetIO() };
	
	// seed random number generation
	srand((unsigned int)time(0));

	std::set<Handle> handlesToDelete{}; //delete message 

	Generation::System::begin(beingManager, generationManager, selectedTracker, textureLibrary);

	while (gameState == SolengineV2::GameState::PLAY || gameState == SolengineV2::GameState::PAUSE)
	{
		unsigned int deltaTime = timeManager.getDeltaTime();
		UserInput::System::update(inputManager, deltaTime, beingManager, camera, selectedTracker, gameState);

		if (gameState != SolengineV2::GameState::PAUSE)
		{
			Partition::System::update(beingManager, spatialPartition.partition);
			Collider::System::update(beingManager, spatialPartition.partition, generationManager, handlesToDelete, deltaTime, textureLibrary);
			Generation::System::update(beingManager, dataPointManager, generationManager, overlayConfig, selectedTracker, textureLibrary, handlesToDelete);
			Survival::System::update(beingManager, deltaTime, generationManager, textureLibrary);
			Velocity::System::update(beingManager, generationManager, deltaTime);
			Selectable::System::update(beingManager, selectionBox, selectedTracker);
		}

		window.Clear();
		Sprite::System::update(beingManager, backgroundImage, camera, spriteBatch, shaders.colourShading);
		Shape::System::update(selectionBox, camera, shaders.simpleGeometry);
		GUI::System::update(beingManager, dataPointManager, generationManager, overlayConfig, selectedTracker, textureLibrary, window, gameState, deltaTime);

		window.SwapBuffer();
		timeManager.limitFPS();
	}

	return 0;
}
