
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>

//#include "../SolengineV2/SDL.h"
#include "../SolengineV2/ResourceManager.h"

#include "../SolengineV2/ShaderFactory.h"
#include "../SolengineV2/ShaderManager.h"

#include "../SolengineV2/Window.h"

#include "../SolengineV2/TimeManager.h"
#include "../SolengineV2/InputManager.h"

#include "TransformSystem.h"
#include "GraphicsSystem.h"
#include "CameraSystem.h"
#include "UserInputSystem.h"
#include "ColliderSystem.h"
#include "EntityManager.h"
#include "VelocitySystem.h"
#include "HandleManager.h"
#include "SelectableSystem.h"
#include "FoodSystem.h"
#include "SurvivalSystem.h"
#include <SDLInit.h>



//multiple shaders?
//UI logic
//add FONTS
//shader organisaiton
//particles

//const iterators?

int main(int argc, char** argv)
{
	SolengineV2::initialiseSDL();
	int screenHeight = 1000, screenWidth = 1000;
	SolengineV2::Window                window("SolengineV2", screenWidth, screenHeight, SolengineV2::Colour(0, 0, 0, 255));
	SolengineV2::IOManager             iOManager;
	SolengineV2::ResourceManager       resourceManager(&iOManager);
	SolengineV2::ShaderManager         shaderManager;
	SolengineV2::ShaderFactory         shaderFactory(&shaderManager);
	SolengineV2::ShaderProgram         shaderProgram;
	shaderFactory.CreateShader("colourShading", "Shaders/colourShading.vert", "Shaders/colourShading.frag", { "vertexPosition", "vertexColour", "vertexUV" });
	SolengineV2::TimeManager           timeManager(6000, true);
	SolengineV2::InputManager          inputManager;

	EntityManager      entityManager;
	HandleManager      handleManager;
	TransformSystem    transformSystem; 
	VelocitySystem     velocitySystem    (&transformSystem);
	CameraSystem       cameraSystem      (&transformSystem, shaderManager.GetShader("colourShading"), &shaderManager, screenHeight, screenWidth); // camera
	SelectableSystem   selectableSystem  (&transformSystem, &cameraSystem, &inputManager);
	GraphicsSystem     graphicsSystem    (&transformSystem, &cameraSystem); // graphics
	HealthSystem       healthSystem      (&transformSystem);
	UserInputSystem    userInputSystem   (&transformSystem, &cameraSystem, &healthSystem, &graphicsSystem, &inputManager);
	FoodSystem         foodSystem        (&transformSystem);
	SurvivalSystem     survivalSystem    (&transformSystem, &velocitySystem, &foodSystem, &graphicsSystem);
	ColliderSystem     colliderSystem(&transformSystem, &survivalSystem);

	graphicsSystem.SetSelectionBoxTextureID(resourceManager.GetTexture("Textures/SelectionBox.png").ID);

	entityManager.Init(
		shaderManager.GetShader("colourShading"),
		&resourceManager,
		&transformSystem, 
		&handleManager, 
		&cameraSystem, 
		&selectableSystem,
		&graphicsSystem, 
		&userInputSystem, 
		&colliderSystem, 
		&velocitySystem, 
		&healthSystem, 
	    &foodSystem,
		&survivalSystem
		);

	float physicsSpeed = 1.0f;
	while (true)
	{
		int adjustedDeltaTicks = timeManager.GetDeltaTicks() * physicsSpeed;
		
		inputManager.ProcessInput();
		entityManager.Process();

		userInputSystem.Process(adjustedDeltaTicks);	
		velocitySystem.Process(adjustedDeltaTicks);
		colliderSystem.Process();
		selectableSystem.Process();
		survivalSystem.Process(adjustedDeltaTicks);

		window.Clear();
		graphicsSystem.Process();


		window.SwapBuffer();
		timeManager.LimitFPS();
	}


	return 0;
}
