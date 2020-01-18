#include <time.h>
#include "GraphicsManager.h"
#include "CameraManager.h"
#include "ColliderManager.h"
#include "UserInputManager.h"
#include <ResourceManager.h>
#include "VelocityManager.h"
#include "AutoShootManager.h"
#include "LifetimeManager.h"
#include "UserShootManager.h"

class EntityCreator
{
public:
	EntityCreator() :
		tManager(nullptr),
		camManager(nullptr),
		gManager(nullptr),
		uManager(nullptr),
		colManager(nullptr),
		rManager(nullptr),
		vManager(nullptr),
		asManager(nullptr),
		usManager(nullptr),
		ltManager(nullptr)
	{}

	TransformManager* tManager;
	CameraManager* camManager;
	GraphicsManager* gManager;
	UserInputManager* uManager;
	ColliderManager* colManager;
	SolengineV2::ResourceManager* rManager;
	VelocityManager* vManager;
	AutoShootManager* asManager;
	UserShootManager* usManager;
	LifetimeManager* ltManager;

	void Init(TransformManager* tm,
		CameraManager* camm,
		GraphicsManager* gm,
		UserInputManager* um,
		ColliderManager* colm,
		SolengineV2::ResourceManager* rm,
		VelocityManager* vman,
		AutoShootManager* asman,
		UserShootManager* usman,
		LifetimeManager* ltman)
	{
		tManager = tm;
		camManager = camm;
		gManager = gm;
		uManager = um;
		colManager = colm;
		rManager = rm;
		vManager = vman;
		asManager = asman;
		usManager = usman;
		ltManager = ltman;

		CreateFloor({ 0.0f, 0.0f, 0.0f });
	}

	void CreatePlayer()
	{
		tManager->AddComponent({ 0.0, 0.0f, 5.0f }, { 20.0f, 20.0f, 0.0f });
		gManager->AddComponent(rManager->GetTexture("Textures/Circle.png").ID, SolengineV2::Colour(0, 255, 0, 255));
		camManager->AddComponent();
		uManager->AddComponent();
		colManager->AddComponent();
		usManager->AddComponent();
		//tManager->SetDeletable();
	}

	void Generate()
	{
		srand(static_cast <unsigned> (time(0)));

		for (int i = 0; i < 25; i++)
		{
			float r1 = 300 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (300)));
			float r2 = 300 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (300)));
			float r3 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (300)));
			float r4 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (300)));

			CreatePinkSquare({ r1, r2 , 0.0f });
			CreateBlueCircle({ r3, r4, 0.0f });
		}
	}

	void CreatePinkSquare(glm::vec3 pos)
	{
		Team team = Team::ONE;
		tManager->AddComponent(pos, { 20.0f, 20.0f, 0.0f });
		gManager->AddComponent(rManager->GetTexture("Textures/Square.png").ID, SolengineV2::Colour(255, 0, 255, 255));
		colManager->AddComponent(ColliderType::CIRCULAR, team);
		tManager->SetDeletable();
		asManager->AddComponent(team, glm::normalize(glm::vec2{ -1, -1 }));
		//piManager->AddComponent();
	}

	void CreateFloor(glm::vec3 pos)
	{
		tManager->AddComponent(pos, { 1920.0f, 1080.0f, -10.0f });
		gManager->AddComponent(rManager->GetTexture("Textures/starf.png").ID, SolengineV2::Colour(255, 255, 255, 100));
	}

	void CreateBlueCircle(glm::vec3 pos)
	{
		Team team = Team::TWO;
		tManager->AddComponent(pos, { 20.0f, 20.0f, 0.0f });
		gManager->AddComponent(rManager->GetTexture("Textures/Circle.png").ID, SolengineV2::Colour(0, 255, 255, 255));
		colManager->AddComponent(ColliderType::CIRCULAR, team);
		tManager->SetDeletable();
		asManager->AddComponent(team, glm::normalize(glm::vec2{ 1, 1 }));
	}

	void CreateBullets(const std::vector<BulletCreationData>& bulData)
	{
		for (BulletCreationData b : bulData)
		{
			tManager->AddComponent(b.pos, { 10.0f, 10.0f, 0.0f });
			gManager->AddComponent(rManager->GetTexture("Textures/bul.png").ID, SolengineV2::Colour(255, 0, 0, 255));
			colManager->AddComponent(ColliderType::POINT, b.team);
			tManager->SetDeletable();
			vManager->AddComponent(b.vel, b.dir);
			ltManager->AddComponent(50000);
		}
	}

	void CreateRandom(const std::vector<glm::vec2>& pos)
	{
		for (glm::vec2 p : pos)
		{
			tManager->AddComponent({ p.x, p.y, 0.0f }, { 15.0f, 15.0f, 15.0f });
			gManager->AddComponent(rManager->GetTexture("Textures/Circle.png").ID, SolengineV2::Colour(0, 255, 0, 255));
			tManager->SetDeletable();
			colManager->AddComponent();
		}
	}

	void Process() //simple concept for generating new entities in game.
	{
		if (uManager->shouldGenerate)
		{
			Generate();
		}

		if (!asManager->NewBullets.empty())
		{
			CreateBullets(asManager->NewBullets);
			asManager->NewBullets.clear();
		}

		if (!usManager->NewBullets.empty())
		{
			CreateBullets(usManager->NewBullets);
			usManager->NewBullets.clear();
		}

		if (!uManager->newpos.empty())
		{
			CreateRandom(uManager->newpos);
			uManager->newpos.clear();
		}

		uManager->shouldGenerate = false;
	}
};