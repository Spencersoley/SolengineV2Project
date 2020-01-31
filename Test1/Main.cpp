#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>

#include <SDL.h>
#include <ResourceManager.h>

#include <ShaderCompiler.h>
#include <Window.h>

#include <TimeManager.h>
#include <InputManager.h>
#include "TransformSystem.h"
#include "GraphicsSystem.h"
#include "CameraSystem.h"
#include "UserInputSystem.h"
#include "ColliderSystem.h"
#include "EntityCreator.h"
#include "VelocitySystem.h"
#include "ShootSystem.h"
#include "LifetimeSystem.h"
#include "EntityDeleter.h"
#include "HandleManager.h"
#include "SelectableSystem.h"

//multiple shaders?
//UI logic
//add FONTS
//shader organisaiton
//particles

int main(int argc, char** argv)
{
	SolengineV2::initialiseSDL();
	int screenHeight = 1000, screenWidth = 1000;
	SolengineV2::Window                window("SolengineV2", screenWidth, screenHeight, SolengineV2::Colour(0, 0, 0, 255));
	SolengineV2::IOManager             iOManager;
	SolengineV2::ResourceManager       resourceManager(&iOManager);
	SolengineV2::ShaderCompiler        shaderCompiler(&iOManager);
	SolengineV2::ShaderProgramManager  shaderManager;
	SolengineV2::ShaderProgram         shaderProgram;
	shaderCompiler.CompileShaders(shaderProgram, "Shaders/colourShading.vert", "Shaders/colourShading.frag", { "vertexPosition", "vertexColour", "vertexUV" });
	SolengineV2::TimeManager           timeManager(6000, true);
	SolengineV2::InputManager          inputManager;

	EntityCreator      creator;
	EntityDeleter      deleter;
	HandleManager      handleManager;
	TransformSystem    transformSystem; 
	VelocitySystem     velocitySystem    (&transformSystem);
	CameraSystem       cameraSystem      (&transformSystem, &shaderProgram, &shaderManager, screenHeight, screenWidth); // camera
	SelectableSystem   selectableSystem  (&transformSystem, &cameraSystem, &inputManager);
	GraphicsSystem     graphicsSystem    (&transformSystem, &cameraSystem); // graphics
	UserInputSystem   userInputSystem   (&transformSystem, &cameraSystem, &inputManager);
	HealthSystem       healthSystem      (&transformSystem);
	ColliderSystem     colliderSystem    (&transformSystem, &healthSystem);
	TargetableSystem   targetableSystem  (&transformSystem);
	ShootSystem        shootSystem       (&transformSystem, &targetableSystem, &cameraSystem, &inputManager);
	LifetimeSystem     lifetimeSystem    (&transformSystem);

	graphicsSystem.SetSelectionBoxTextureID(resourceManager.GetTexture("Textures/SelectionBox.png").ID);

	creator.Init(
		&shaderProgram,
		&transformSystem, 
		&handleManager, 
		&cameraSystem, 
		&selectableSystem,
		&graphicsSystem, 
		&userInputSystem, 
		&colliderSystem, 
		&resourceManager, 
		&velocitySystem, 
		&shootSystem, 
		&lifetimeSystem, 
		&healthSystem, 
		&targetableSystem);
	deleter.Init(
		&transformSystem, 
		&handleManager, 
		&cameraSystem, 
		&selectableSystem,
		&graphicsSystem, 
		&userInputSystem,
		&colliderSystem, 
		&velocitySystem, 
		&shootSystem, 
		&lifetimeSystem, 
		&healthSystem, 
		&targetableSystem);

	creator.CreatePlayer();
	//creator.Generate();

	float physicsSpeed = 1.0f;
	while (true)
	{
		int adjustedDeltaTicks = timeManager.GetDeltaTicks() * physicsSpeed;
		
		inputManager.ProcessInput();

		creator.Process();
		userInputSystem.Process(adjustedDeltaTicks);	
		velocitySystem.Process(adjustedDeltaTicks);
		colliderSystem.Process();
		shootSystem.Process(adjustedDeltaTicks);
		lifetimeSystem.Process(adjustedDeltaTicks);
		//selectableSystem.Process();

		deleter.Process();

		window.Clear();
		graphicsSystem.Process();

		window.SwapBuffer();
		timeManager.LimitFPS();
	}
	return 0;
}
