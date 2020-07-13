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

#include "TransformSystem.h"
#include "CameraSystem.h"
#include "UserInputSystem.h"
#include "ColliderSystem.h"
#include "VelocitySystem.h"
#include "SelectableSystem.h"
#include "SurvivalSystem.h"
#include "SpriteSystem.h"
#include "BeingManager.h"
#include "GeneSystem.h"
#include "GenerationSystem.h"
#include "GUISystem.h"
#include "DataSystem.h"
#include "PartitionSystem.h"

#include "Sceensize.h"
#include "ShapeSystem.h"

// change the way animals seek food - carnivores, herbivores will ignore the wrong food types
// the right food types provide huge bonuses 

// make strength do something

// can we merge begin and new wave in some way?

// compare inline vs non inline?

// change data duplication system to a resource rather than a parameter?

// selected entity plant saturation/fullness

// choose active traits at start of simulation
// make optional all traits through a configurationSystem

// convert time system from nanosecond precision to microsecond precision - we don't need nano?

// try being implementation of being manager

constexpr double FRAMERATE_ADJUSTMENT = 0.00001;

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

	struct Textures
	{
		SolengineV2::Texture circle;
		SolengineV2::Texture background;
	} textures;

	SolengineV2::Window            window("SolengineV2", SCREEN_WIDTH, SCREEN_HEIGHT, SolengineV2::Colour(0, 0, 0, 255));
	SolengineV2::ShaderManager     shaderManager;
	SolengineV2::GeometryRenderer  renderer;
	SolengineV2::TimeManager       timeManager(100000, true);
	SolengineV2::InputManager      inputManager;

	{
		SolengineV2::IOManager         ioManager;
		SolengineV2::ShaderCreator     shaderCreator(ioManager);
		shaderCreator.createShader(shaders.colourShading, "Shaders/colourShading.vert", "Shaders/colourShading.frag", { "vertexPosition", "vertexColour", "vertexUV" });
		shaderCreator.createShader(shaders.simpleGeometry, "Shaders/simpleGeometry.vert", "Shaders/simpleGeometry.frag", { "aPos" });

		SolengineV2::ImageLoader       imageLoader(ioManager);
		imageLoader.loadPNG(textures.circle, "Textures/Circle.png");
		imageLoader.loadPNG(textures.background, "Textures/background1.png");
	}

	SolengineV2::GameState gameState{ SolengineV2::GameState::PLAY };

	TransformSystem  transformSystem;
	DataSystem       dataSystem;

	PartitionSystem  partitionSystem(transformSystem);
	VelocitySystem   velocitySystem(transformSystem);
	CameraSystem     cameraSystem(transformSystem);
	GeneSystem       geneSystem;
	SelectableSystem selectableSystem(transformSystem);

	SpriteSystem     spriteSystem(shaderManager, shaders.colourShading, cameraSystem, transformSystem, textures.background.ID);
	SurvivalSystem   survivalSystem(transformSystem, velocitySystem, geneSystem, spriteSystem, partitionSystem);
	ColliderSystem   colliderSystem(transformSystem, survivalSystem, geneSystem, partitionSystem, velocitySystem);
	ShapeSystem      shapeSystem(renderer, shaderManager, shaders.simpleGeometry, transformSystem, cameraSystem, selectableSystem);
	GenerationSystem generationSystem(selectableSystem, survivalSystem, dataSystem, geneSystem);

	UserInputSystem  userInputSystem(inputManager, transformSystem, cameraSystem, selectableSystem);
	GUISystem        guiSystem(window, velocitySystem, generationSystem, dataSystem, geneSystem, selectableSystem, survivalSystem);

	//beings
	BeingManager beingManager(textures.circle.ID);

	srand((unsigned int)time(0));

	generationSystem.begin(beingManager);
	
	while (gameState == SolengineV2::GameState::PLAY || gameState == SolengineV2::GameState::PAUSE)
	{
		long long deltaTime = timeManager.getDeltaTime();

		userInputSystem.process(deltaTime* FRAMERATE_ADJUSTMENT, beingManager, gameState);

		if (gameState != SolengineV2::GameState::PAUSE)
		{
			generationSystem.process(beingManager);
			selectableSystem.process(beingManager);
			velocitySystem.process(beingManager, deltaTime);
			colliderSystem.process(beingManager, deltaTime);
			survivalSystem.process(beingManager, deltaTime);
		}

		window.Clear();
		spriteSystem.process(beingManager);
		shapeSystem.process(beingManager);

		guiSystem.process(beingManager, gameState);
		
		window.SwapBuffer();
		timeManager.limitFPS(true);
	}

	return 0;
}
