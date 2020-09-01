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
#include <RandomNumberGenerator.h>

#include "Sceensize.h"
#include "GameData.h"

#include "CameraSystemSpecialization.h"
#include "ColliderSystemSpecialization.h"
#include "DataSystemSpecialization.h"
#include "GenerationSystemSpecialization.h"
#include "GUISystemSpecialization.h"
#include "PartitionSystemSpecialization.h"
#include "SelectableSystemSpecialization.h"
#include "ShapeSystemSpecialization.h"
#include "SpriteSystemSpecialization.h"
#include "SurvivalSystemSpecialization.h"
#include "UserInputSystemSpecialization.h"	
#include "SpeedTraitSystemSpecialization.h"
#include "TextureLoaderSystemSpecialization.h"

class EvoSim
{
public:
	void run()
	{
		init();
		loop();
	}

private:
	SolengineV2::Window                 window{};
	SolengineV2::ShaderManager          shaderManager{};
	SolengineV2::TimeManager            timeManager{};
	SolengineV2::InputManager           inputManager{};
	SolengineV2::SpriteBatch            spriteBatch{};
	SolengineV2::GameState              gameState{ SolengineV2::GameState::BEGIN };
	SolengineV2::RandomNumberGenerator  rng{};
	struct Shaders
	{
		SolengineV2::ShaderProgram colourShading;
		SolengineV2::ShaderProgram simpleGeometry;
	} shaders;

	ImGuiIO io;
	GameData gameData{};

	void init()
	{
		SolengineV2::initialiseSDL();
		window.Init("SolengineV2", SCREEN_WIDTH, SCREEN_HEIGHT, SolengineV2::Colour(0, 0, 0, 255));
		timeManager.Init(100000, SolengineV2::TimerMode::NONE);
		spriteBatch.Init();

		SolengineV2::IOManager         ioManager;
		SolengineV2::ShaderCreator     shaderCreator(ioManager);
		shaderCreator.createShader(shaders.colourShading, "../deps/include/Shaders/colourShading.vert", "../deps/include/Shaders/colourShading.frag", { "vertexPosition", "vertexColour", "vertexUV" });
		shaderCreator.createShader(shaders.simpleGeometry, "../deps/include/Shaders/simpleGeometry.vert", "../deps/include/Shaders/simpleGeometry.frag", { "aPos" });
		TextureLoader::System::loadTextures(ioManager, gameData);

		GUI::System::init(window);
		io = ImGui::GetIO();

		SolengineV2::GameState gameState{ SolengineV2::GameState::PLAY };

		Transform::System::setPos(gameData.selectionBox.transform, glm::vec2(FLT_MAX));
		Transform::System::setDims(gameData.selectionBox.transform, glm::vec2(25.0f));

		Transform::System::setPos(gameData.camera.transform, { -0.0f, 0.0f });
		Transform::System::setDims(gameData.camera.transform, { 0.0f, 0.0f });

		Transform::System::setPos(gameData.backgroundImage.transform, { 0.0f, 0.0f });
		auto arenaDiameter = gameData.configurableSettings.arenaDiameter.get();
		Transform::System::setDims(gameData.backgroundImage.transform, { arenaDiameter, arenaDiameter });
		Sprite::System::setColour(gameData.backgroundImage.sprite, { 255, 255, 255, 255 });
		Sprite::System::setTextureID(gameData.backgroundImage.sprite, TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::BACKGROUND, gameData));

		Shape::System::initSquare(gameData.selectionBox.shape);

		Partition::System::setupGrid(gameData.spatialPartition.partition, gameData);

		srand((unsigned int)time(0));
	}

	void loop()
	{
		while (gameState != SolengineV2::GameState::EXIT)
		{
			auto deltaTime = timeManager.getDeltaTime<std::chrono::microseconds>();
			UserInput::System::update(inputManager, deltaTime, gameState, gameData);

			if (gameState == SolengineV2::GameState::BEGIN)
			{
				Data::System::reset(gameData.dataPoint);
				Generation::System::begin(gameData, rng);
				gameState = SolengineV2::GameState::PLAY;
			}

			if (gameState == SolengineV2::GameState::PLAY)
			{
				Generation::System::update(gameData, rng, deltaTime);
				Partition::System::update(gameData);
				Collider::System::update(deltaTime, gameData);
				Survival::System::update(deltaTime, gameData);
				SpeedTrait::System::update(deltaTime, gameData);
				Selectable::System::update(gameData);
			}

			window.Clear();
			Sprite::System::update(spriteBatch, shaders.colourShading, gameData);
			Shape::System::update(shaders.simpleGeometry, gameData);
			GUI::System::update(window, gameState, deltaTime, gameData);

			window.SwapBuffer();
			timeManager.limitFPS();
		}
	}
};

int main(int argc, char** argv)
{
	EvoSim evoSim;
	evoSim.run();

	return 0;
}

