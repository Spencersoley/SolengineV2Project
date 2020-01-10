#include "GraphicsManager.h"
#include "CameraManager.h"
#include "ColliderManager.h"
#include "PlayerInputManager.h"
#include <ResourceManager.h>
#include <time.h>

class EntityCreator
{
public:
	EntityCreator() :
		tManager(nullptr),
		camManager(nullptr),
		gManager(nullptr),
		piManager(nullptr),
		colManager(nullptr),
		rManager(nullptr)
	{}

	TransformManager* tManager;
	CameraManager* camManager;
	GraphicsManager* gManager;
	PlayerInputManager* piManager;
	ColliderManager* colManager;
	SolengineV2::ResourceManager* rManager;

	void Init(TransformManager* tm,
		CameraManager* camm,
		GraphicsManager* gm,
		PlayerInputManager* pim,
		ColliderManager* colm,
		SolengineV2::ResourceManager* rm)
	{
		tManager = tm;
		camManager = camm;
		gManager = gm;
		piManager = pim;
		colManager = colm;
		rManager = rm;
	}

	void CreatePlayer()
	{
		tManager->AddComponent({ 0.0, 0.0f, 5.0f }, { 20.0f, 20.0f, 0.0f });
		gManager->AddComponent(rManager->GetTexture("Textures/Circle.png").ID, SolengineV2::Colour(255, 255, 0, 255));
		camManager->AddComponent();
		piManager->AddComponent();
		colManager->AddComponent();
		tManager->SetDeletable();
	}

	void Generate()
	{
		srand(static_cast <unsigned> (time(0)));

		for (int i = 0; i < 25; i++)
		{
			float r1 = 300 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (300)));
			float r2 = 300 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (300)));
			float r3 = 300 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (300)));
			float r4 = 300 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (300)));

			CreatePinkSquare({ r1, r2 , 0.0f });
			CreateBlueCircle({ r3, r4, 0.0f });
		}
	}

	void CreatePinkSquare(glm::vec3 pos)
	{
		tManager->AddComponent(pos, { 10.0f, 10.0f, 10.0f });
		gManager->AddComponent(rManager->GetTexture("Textures/Square.png").ID, SolengineV2::Colour(255, 0, 255, 255));
		colManager->AddComponent();
		tManager->SetDeletable();
		//piManager->AddComponent();
	}

	void CreateBlueCircle(glm::vec3 pos)
	{
		tManager->AddComponent(pos, { 10.0f, 10.0f, 0.0f });
		gManager->AddComponent(rManager->GetTexture("Textures/Circle.png").ID, SolengineV2::Colour(0, 255, 255, 255));
		colManager->AddComponent();
		tManager->SetDeletable();
		camManager->AddComponent();
	}

	void Process() //simple concept for generating new entities in game.
	{
		if (piManager->shouldGenerate)
		{
			Generate();
		}
		piManager->shouldGenerate = false;
	}
};