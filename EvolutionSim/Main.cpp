#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <set>

#include <ImageLoader.h>
#include <ShaderCreator.h>
#include <ShaderManager.h>
#include <Window.h>
#include <TimeManager.h>
#include <InputManager.h>
#include <SDLInit.h>
#include <GeometryRenderer.h>

#include "BeingManager.h"
#include "BackgroundImage.h"
#include "Camera.h"
#include "SelectionBox.h"

#include "CameraSystem.h"
#include "ColliderSystem.h"
#include "DataSystem.h"
#include "GeneSystem.h"
#include "GenerationSystem.h"
#include "GUISystem.h"
#include "OverlaySystem.h"
#include "PartitionSystem.h"
#include "SelectableSystem.h"
#include "ShapeSystem.h"
#include "SpriteSystem.h"
#include "SurvivalSystem.h"
#include "TransformSystem.h"
#include "UserInputSystem.h"
#include "VelocitySystem.h"

#include "Sceensize.h"
#include "ArenaSize.h"
#include "TextureSystem.h"

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

template <typename E>
constexpr auto to_underlying(E e) noexcept
{
	return static_cast<std::underlying_type_t<E>>(e);
}

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
	SolengineV2::GeometryRenderer  renderer;
	SolengineV2::TimeManager       timeManager(100000, SolengineV2::TimerMode::LIFETIME_AVERAGE);
	SolengineV2::InputManager      inputManager;
	
	TextureSystem textureSystem;

	{
		SolengineV2::IOManager         ioManager;
		SolengineV2::ShaderCreator     shaderCreator(ioManager);
		shaderCreator.createShader(shaders.colourShading, "Shaders/colourShading.vert", "Shaders/colourShading.frag", { "vertexPosition", "vertexColour", "vertexUV" });
		shaderCreator.createShader(shaders.simpleGeometry, "Shaders/simpleGeometry.vert", "Shaders/simpleGeometry.frag", { "aPos" });
		textureSystem.init(ioManager);
	}

	//Systems
	TransformSystem transformSystem;
	DataSystem dataSystem;

	PartitionSystem partitionSystem(
		transformSystem
	);

	VelocitySystem velocitySystem(
		transformSystem
	);

	CameraSystem cameraSystem(
		transformSystem
	);

	GeneSystem geneSystem;

	SelectableSystem selectableSystem(
		transformSystem
	);

	SpriteSystem spriteSystem(
		shaderManager, 
		shaders.colourShading, 
		cameraSystem, 
		transformSystem
	);

	SurvivalSystem survivalSystem(
		textureSystem, 
		transformSystem, 
		velocitySystem, 
		geneSystem, 
		spriteSystem
	);

	ColliderSystem colliderSystem(
		geneSystem,
		partitionSystem,
		spriteSystem,
		survivalSystem,
		textureSystem, 
		transformSystem,
		velocitySystem
	);

	ShapeSystem shapeSystem(
		renderer, 
		shaderManager, 
		shaders.simpleGeometry, 
		transformSystem, 
		cameraSystem, 
		selectableSystem
	);

	OverlaySystem overlaySystem(
		spriteSystem, 
		geneSystem, 
		survivalSystem
	);

	GenerationSystem generationSystem(
		colliderSystem,
		dataSystem,
		geneSystem,
		overlaySystem,
		selectableSystem,
		spriteSystem,
		survivalSystem,
		textureSystem,
		transformSystem
	);

	UserInputSystem userInputSystem(
		inputManager, 
		transformSystem, 
		cameraSystem, 
		selectableSystem
	);

	GUISystem guiSystem(
		window, 
		textureSystem, 
		velocitySystem, 
		generationSystem,
		dataSystem,
		overlaySystem, 
		geneSystem, 
		selectableSystem, 
		survivalSystem
	);

	SolengineV2::GameState gameState{ SolengineV2::GameState::PLAY };

	//Entities
	BeingManager     beingManager(geneSystem, spriteSystem, velocitySystem, survivalSystem, transformSystem);

	SelectionBox     selectionBox; 
	transformSystem.setPos(selectionBox.transform, glm::vec2(FLT_MAX));
	transformSystem.setDims(selectionBox.transform, glm::vec2(25.0f));

	Camera           camera;
	transformSystem.setPos(camera.transform, { -300.0f, 0.0f });
	transformSystem.setDims(camera.transform, { 0.0f, 0.0f });

	BackgroundImage  backgroundImage;
	transformSystem.setPos(backgroundImage.transform, { 0.0f, 0.0f });
	transformSystem.setDims(backgroundImage.transform, { ARENA_SIZE, ARENA_SIZE });
	spriteSystem.setColour(backgroundImage.sprite, { 255, 255, 255, 255 });
	spriteSystem.setTextureID(backgroundImage.sprite, textureSystem.getTextureID(TextureSystem::Component::BACKGROUND));

	// seed random number generation
	srand((unsigned int)time(0));

	generationSystem.begin(beingManager);

	while (gameState == SolengineV2::GameState::PLAY || gameState == SolengineV2::GameState::PAUSE)
	{
		unsigned int deltaTime = timeManager.getDeltaTime();

		userInputSystem.update(deltaTime, beingManager, camera, gameState);

		if (gameState != SolengineV2::GameState::PAUSE)
		{
			colliderSystem.update(beingManager, deltaTime);
			generationSystem.update(beingManager);
			survivalSystem.update(beingManager, deltaTime);
			velocitySystem.update(beingManager, deltaTime);

			selectableSystem.update(beingManager, selectionBox);
		}

		window.Clear();
		spriteSystem.update(beingManager, backgroundImage, camera);
		shapeSystem.update(selectionBox, camera);

		guiSystem.update(beingManager, gameState, deltaTime);

		window.SwapBuffer();
		timeManager.limitFPS();
	}

	return 0;
}
