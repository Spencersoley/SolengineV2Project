
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>

#include <ResourceManager.h>
#include <ShaderFactory.h>
#include <ShaderManager.h>
#include <Window.h>
#include <TimeManager.h>
#include <InputManager.h>
#include <SDLInit.h>

#include "TransformSystem.h"
#include "CameraSystem.h"
#include "UserInputSystem.h"
#include "ColliderSystem.h"
#include "EntityManager.h"
#include "VelocitySystem.h"
#include "HandleManager.h"
#include "SelectableSystem.h"
#include "FoodSystem.h"
#include "SurvivalSystem.h"
#include "SpriteSystem.h"
#include "TextSystem.h"

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
	SolengineV2::TimeManager           timeManager(6000, true);
	SolengineV2::InputManager          inputManager;

	shaderFactory.CreateShader("colourShading", "Shaders/colourShading.vert", "Shaders/colourShading.frag", { "vertexPosition", "vertexColour", "vertexUV" });

	EntityManager      entityManager;
	HandleManager      handleManager;
	TransformSystem    transformSystem; 
	VelocitySystem     velocitySystem    (&transformSystem);
	CameraSystem       cameraSystem      (&transformSystem, shaderManager.GetShader("colourShading"), &shaderManager, screenHeight, screenWidth); 
	SelectableSystem   selectableSystem  (&transformSystem, &cameraSystem, &inputManager);
	SpriteSystem       spriteSystem      (&transformSystem, &cameraSystem); 
	HealthSystem       healthSystem      (&transformSystem);
	TextSystem         textSystem        (&transformSystem, &cameraSystem, shaderManager.GetShader("colourShading"));
	UserInputSystem    userInputSystem   (&transformSystem, &cameraSystem, &healthSystem, &spriteSystem, &inputManager);
	FoodSystem         foodSystem        (&transformSystem);
	SurvivalSystem     survivalSystem    (&transformSystem, &velocitySystem, &foodSystem, &spriteSystem, &textSystem);
	ColliderSystem     colliderSystem    (&transformSystem, &survivalSystem);

	spriteSystem.SetSelectionBoxTextureID(resourceManager.GetTexture("Textures/SelectionBox.png").ID);

	entityManager.Init(
		shaderManager.GetShader("colourShading"),
		&resourceManager,
		&transformSystem, 
		&handleManager, 
		&cameraSystem, 
		&selectableSystem,
		&spriteSystem, 
		&textSystem,
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
		spriteSystem.Process();
		textSystem.Process();

		window.SwapBuffer();
		timeManager.LimitFPS();
	}

	return 0;
}
