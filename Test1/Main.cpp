#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>

#include <SDL.h>
#include <ResourceManager.h>

#include <ShaderCompiler.h>
#include <Window.h>

#include <TimeManager.h>
#include <InputManager.h>
#include "TransformManager.h"
#include "GraphicsManager.h"
#include "CameraManager.h"
#include "UserInputManager.h"
#include "ColliderManager.h"
#include "EntityCreator.h"
#include "VelocityManager.h"
#include "AutoShootManager.h"
#include "LifetimeManager.h"
#include "UserShootManager.h"

int main(int argc, char** argv)
{
	SolengineV2::initialiseSDL();
	int screenHeight = 1000, screenWidth = 1000;
	SolengineV2::Window window("SolengineV2", screenWidth, screenHeight, 0, SolengineV2::Colour(0, 0, 0, 255));

	SolengineV2::IOManager iOManager;
	SolengineV2::ResourceManager resourceManager(&iOManager);
	SolengineV2::ShaderCompiler shaderCompiler(&iOManager);
	SolengineV2::ShaderProgram shaderProgram;
	shaderCompiler.CompileShaders(&shaderProgram, "Shaders/colourShading.vert", "Shaders/colourShading.frag", { "vertexPosition", "vertexColour", "vertexUV" });
	SolengineV2::TimeManager timeManager(6000, true);
	SolengineV2::InputManager inputManager;

	EntityCreator creator;
	TransformManager tManager; // transform
	VelocityManager vManager(&tManager);
	CameraManager camManager(&tManager, &shaderProgram, screenHeight, screenWidth); // camera
	GraphicsManager gManager(&tManager, &camManager); // graphics
	UserInputManager piManager(&tManager, &camManager, &inputManager);//, &creator);
	UserShootManager usManager(&tManager, &camManager, &inputManager);
	ColliderManager colManager(&tManager);
	AutoShootManager asManager(&tManager);
	LifetimeManager ltManager(&tManager);

	creator.Init(&tManager, &camManager, &gManager, &piManager, &colManager, &resourceManager, &vManager, &asManager, &usManager, &ltManager);

	creator.CreatePlayer();
	//creator.Generate();

	float physicsSpeed = 1.0f;
	gManager.Init();
	while (true)
	{
		int adjustedDeltaTicks = timeManager.GetDeltaTicks() * physicsSpeed;
		
		inputManager.ProcessInput();

		creator.Process();
		piManager.Process(adjustedDeltaTicks);	
		vManager.Process(adjustedDeltaTicks);
		colManager.Process();
		asManager.Process(adjustedDeltaTicks);
		usManager.Process(adjustedDeltaTicks);
		ltManager.Process(adjustedDeltaTicks);


		tManager.Process();
		camManager.Process();
		gManager.Process();

	


		shaderProgram.Unuse();
		window.SwapBuffer();
		timeManager.LimitFPS(true);
	}
	return 0;
}
