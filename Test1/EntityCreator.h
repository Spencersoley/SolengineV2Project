#include <time.h>
#include <ResourceManager.h>
#include "GraphicsSystem.h"
#include "CameraSystem.h"
#include "ColliderSystem.h"
#include "UserInputSystem.h"
#include "VelocitySystem.h"
#include "ShootSystem.h"
#include "LifetimeSystem.h"
#include "TargetableSystem.h"
#include "HandleManager.h"
#include "SelectableSystem.h"

class EntityCreator
{
public:
	EntityCreator() :
		shaderProgram(nullptr),
		transformSystem(nullptr),
		handleManager(nullptr),
		cameraSystem(nullptr),
		selectableSystem(nullptr),
		graphicsSystem(nullptr),
		userInputSystem(nullptr),
		colliderSystem(nullptr),
		resourceManager(nullptr),
		velocitySystem(nullptr),
		shootSystem(nullptr),
		lifetimeSystem(nullptr),
		healthSystem(nullptr),
		targetableSystem(nullptr)
	{}

	//std::map<std::string, std::shared_ptr<System>> systemMap;

	SolengineV2::ShaderProgram* shaderProgram;

	TransformSystem* transformSystem;
	HandleManager* handleManager;
	CameraSystem* cameraSystem;
	SelectableSystem* selectableSystem;
	GraphicsSystem* graphicsSystem;
	UserInputSystem* userInputSystem;
	ColliderSystem* colliderSystem;
	SolengineV2::ResourceManager* resourceManager;
	VelocitySystem* velocitySystem;
	ShootSystem* shootSystem;
	LifetimeSystem* lifetimeSystem;
	HealthSystem* healthSystem;
	TargetableSystem* targetableSystem;

	//void Init(SolengineV2::ShaderProgram* shader,
	//	HandleManager* handle,
	//	std::map<std::string, System>
	//)
	//{
	//	shaderProgram = shader;
	//	handleManager = handle;
	//}

	void Init(SolengineV2::ShaderProgram* shader,
		TransformSystem* transform,
		HandleManager* handle,
		CameraSystem* camera,
		SelectableSystem* selectable,
		GraphicsSystem* graphics,
		UserInputSystem* userinput,
		ColliderSystem* collider,
		SolengineV2::ResourceManager* rm,
		VelocitySystem* velocity,
		ShootSystem* shoot,
		LifetimeSystem* lifetime,
		HealthSystem* health,
		TargetableSystem* targetable)
	{
		shaderProgram = shader;
		transformSystem = transform;
		handleManager = handle;
		cameraSystem = camera;
		selectableSystem = selectable;
		graphicsSystem = graphics;
		userInputSystem = userinput;
		colliderSystem = collider;
		resourceManager = rm;
		velocitySystem = velocity;
		shootSystem = shoot;
		lifetimeSystem = lifetime;
		healthSystem = health;
		targetableSystem = targetable;

		CreateFloor({ 0.0f, 0.0f, 0.0f });
	}

	void CreatePlayer()
	{
		Team team = Team::NONE;
		int ID = handleManager->NewHandle();
		transformSystem->AddComponent(ID, { 0.0f, 0.0f, 5.0f }, { 20.0f, 20.0f, 0.0f });
		cameraSystem->AddComponent(ID);
		userInputSystem->AddComponent(ID);
		graphicsSystem->AddComponent(ID, shaderProgram, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/Square.png").ID, SolengineV2::Colour(0, 255, 0, 255));
		colliderSystem->AddComponent(ID, ColliderType::CIRCULAR, Team::NONE);
		targetableSystem->AddComponent(ID, Team::NONE);
		shootSystem->AddComponent(ID, ShootType::PLAYER, Team::NONE);

		int childID = handleManager->NewHandle();
		transformSystem->AddChild(ID, childID, glm::vec3{ 0.0f, -15.0f, 0.0f }, glm::vec3{ 18.0f, 3.0f, 10.0f });
		graphicsSystem->AddComponent(childID, shaderProgram, GraphicsType::WORLDUI, resourceManager->GetTexture("Textures/blank.png").ID, SolengineV2::Colour(0, 150, 0, 155));
		healthSystem->AddComponent(ID, childID);

		int UIID = handleManager->NewHandle();
		transformSystem->AddComponent(UIID, { 140.0f, 40.0f, 5.0f }, { 100.0f, 100.0f, 0.0f });
		graphicsSystem->AddComponent(UIID, shaderProgram, GraphicsType::UI, resourceManager->GetTexture("Textures/blank.png").ID, SolengineV2::Colour(222, 150, 0, 255));
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
		int ID = handleManager->NewHandle();
		Team team = Team::ONE;
		transformSystem->AddComponent(ID, pos, { 20.0f, 20.0f, 0.0f });
		graphicsSystem->AddComponent(ID, shaderProgram, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/Square.png").ID, SolengineV2::Colour(255, 0, 255, 255));
		colliderSystem->AddComponent(ID, ColliderType::CIRCULAR, team);
		shootSystem->AddComponent(ID, ShootType::AUTO, team);
		targetableSystem->AddComponent(ID, team);
		selectableSystem->AddComponent(ID);
		//cameraSystem->AddComponent(ID);

		int childID = handleManager->NewHandle();
		transformSystem->AddChild(ID, childID, glm::vec3{ 0.0f, -15.0f, 0.0f }, glm::vec3{ 18.0f, 3.0f, 10.0f });
		graphicsSystem->AddComponent(childID, shaderProgram, GraphicsType::WORLDUI, resourceManager->GetTexture("Textures/blank.png").ID, SolengineV2::Colour(0, 150, 0, 155));
		healthSystem->AddComponent(ID, childID);
	
		//piManager->AddComponent();
	}

	void CreateFloor(glm::vec3 pos)
	{
		int ID = handleManager->NewHandle();
		transformSystem->AddComponent(ID, pos, { 1920.0f, 1080.0f, -10.0f });
		graphicsSystem->AddComponent(ID, shaderProgram, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/starf.png").ID, SolengineV2::Colour(255, 255, 255, 100));
	}

	void CreateBlueCircle(glm::vec3 pos)
	{
		Team team = Team::TWO;
		int ID = handleManager->NewHandle();
		transformSystem->AddComponent(ID, pos, { 20.0f, 20.0f, 0.0f });
		graphicsSystem->AddComponent(ID, shaderProgram, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/Circle.png").ID, SolengineV2::Colour(0, 255, 255, 255));
		colliderSystem->AddComponent(ID, ColliderType::CIRCULAR, team);
		//healthManager->AddComponent(id);
		shootSystem->AddComponent(ID, ShootType::PLAYER, team);
		targetableSystem->AddComponent(ID, team);
		selectableSystem->AddComponent(ID);

		int childID = handleManager->NewHandle();
		transformSystem->AddChild(ID, childID, glm::vec3{ 0.0f, -15.0f, 0.0f }, glm::vec3{ 18.0f, 3.0f, 10.0f });//, pos, { 10.0f, 10.0f, 0.0f });
		graphicsSystem->AddComponent(childID, shaderProgram, GraphicsType::WORLDUI, resourceManager->GetTexture("Textures/blank.png").ID, SolengineV2::Colour(0, 150, 0, 155));
		healthSystem->AddComponent(ID, childID);	
	}

	void CreateBullets(const std::vector<BulletCreationData>& bulData)
	{
		for (BulletCreationData b : bulData)
		{
			int ID = handleManager->NewHandle();
			transformSystem->AddComponent(ID, b.pos, { 10.0f, 10.0f, 0.0f });
			graphicsSystem->AddComponent(ID, shaderProgram, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/bul.png").ID, SolengineV2::Colour(255, 0, 0, 255));
			colliderSystem->AddComponent(ID, ColliderType::POINT, b.team);
			velocitySystem->AddComponent(ID, b.vel, b.dir);
			lifetimeSystem->AddComponent(ID, 5000);
		}
	}

	void CreateRandom(const std::vector<glm::vec2>& pos)
	{
		for (glm::vec2 p : pos)
		{
			int ID = handleManager->NewHandle();
			transformSystem->AddComponent(ID, { p.x, p.y, 0.0f }, { 15.0f, 15.0f, 15.0f });
			graphicsSystem->AddComponent(ID, shaderProgram, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/Circle.png").ID, SolengineV2::Colour(0, 255, 0, 255));
			colliderSystem->AddComponent(ID);
		}
	}

	void CreateSelectedIcon(std::vector<int>& handles)
	{

	}

	void Process() //simple concept for generating new entities in game.
	{
		if (userInputSystem->shouldGenerate)
		{
			Generate();
		}

		if (!shootSystem->NewBullets.empty())
		{
			CreateBullets(shootSystem->NewBullets);
			shootSystem->NewBullets.clear();
		}

		if (!userInputSystem->newpos.empty())
		{
			CreateRandom(userInputSystem->newpos);
			userInputSystem->newpos.clear();
		}

		userInputSystem->shouldGenerate = false;
	}
};