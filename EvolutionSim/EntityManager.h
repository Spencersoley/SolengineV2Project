#include <time.h>
#include <ResourceManager.h>
#include "GraphicsSystem.h"
#include "CameraSystem.h"
#include "ColliderSystem.h"
#include "UserInputSystem.h"
#include "VelocitySystem.h"
#include "LifetimeSystem.h"
#include "TargetableSystem.h"
#include "HandleManager.h"
#include "SelectableSystem.h"

// implement creator and deleter as a single class
class EntityManager
{
public:
	EntityManager()
		:
		shaderProgram(nullptr),
		resourceManager(nullptr),
		transformSystem(nullptr),
		handleManager(nullptr),
		cameraSystem(nullptr),
		selectableSystem(nullptr),
		graphicsSystem(nullptr),
		userInputSystem(nullptr),
		colliderSystem(nullptr),
		velocitySystem(nullptr),
		lifetimeSystem(nullptr),
		healthSystem(nullptr),
		foodSystem(nullptr),
		survivalSystem(nullptr)
	{}

	void Init(SolengineV2::ShaderProgram* shader,
		SolengineV2::ResourceManager* rm,
		TransformSystem* transform,
		HandleManager* handle,
		CameraSystem* camera,
		SelectableSystem* selectable,
		GraphicsSystem* graphics,
		UserInputSystem* userinput,
		ColliderSystem* collider,
		VelocitySystem* velocity,
		LifetimeSystem* lifetime,
		HealthSystem* health,
		FoodSystem* food,
		SurvivalSystem* survival
	)
	{
		shaderProgram = shader;
		resourceManager = rm;

		transformSystem = transform;
		handleManager = handle;
		cameraSystem = camera;
		selectableSystem = selectable;
		graphicsSystem = graphics;
		userInputSystem = userinput;
		colliderSystem = collider;
		velocitySystem = velocity;
		lifetimeSystem = lifetime;
		healthSystem = health;
		foodSystem = food;
		survivalSystem = survival;

		srand(time(NULL));

		createFloor({ 0.0f, 0.0f, 0.0f });

		createPlayer();
	}

	void Process() //simple concept for generating new entities in game.
	{
		for (auto it = transformSystem->GetHandlesToDeleteBegin(); it != transformSystem->GetHandlesToDeleteEnd(); ++it)
		{
			int handle = (*it);
			handleManager->DeleteHandle(handle);

			cameraSystem->DeleteComponent(handle);
			graphicsSystem->DeleteComponent(handle);
			userInputSystem->DeleteComponent(handle);
			colliderSystem->DeleteComponent(handle);
			velocitySystem->DeleteComponent(handle);
			lifetimeSystem->DeleteComponent(handle);
			healthSystem->DeleteComponent(handle);
			selectableSystem->DeleteComponent(handle);
			foodSystem->DeleteComponent(handle);
			survivalSystem->DeleteComponent(handle);
		}

		transformSystem->ClearHandles();

	/*	if (userInputSystem->shouldGenerateFood)
		{
			generateFood();
			userInputSystem->shouldGenerateFood = false;
		}


		if (userInputSystem->shouldGenerateUnits)
		{
			generateUnits();
			userInputSystem->shouldGenerateUnits = false;
		}*/

		if (survivalSystem->shouldDuplicate)
		{
			generateUnits(survivalSystem->shouldDuplicate);
			generateFood(10);
			survivalSystem->shouldDuplicate = 0;
		}
	}

private:
	SolengineV2::ShaderProgram* shaderProgram;
	SolengineV2::ResourceManager* resourceManager;

	TransformSystem* transformSystem;
	HandleManager* handleManager;
	CameraSystem* cameraSystem;
	SelectableSystem* selectableSystem;
	GraphicsSystem* graphicsSystem;
	UserInputSystem* userInputSystem;
	ColliderSystem* colliderSystem;
	VelocitySystem* velocitySystem;
	LifetimeSystem* lifetimeSystem;
	HealthSystem* healthSystem;
	FoodSystem* foodSystem;
	SurvivalSystem* survivalSystem;

	void generateFood(int num)
	{
		for (int i = 0; i < num; i++)
		{
		    createFood(randomPosInCircle(400.0f));	
		}
	}

	void generateUnits(int num)
	{
		for (int i = 0; i < num; i++)
		{
			createUnit(randomPosOutsideCircle(400.0f));
		}
	}

	glm::vec3 randomPosOutsideCircle(float minRadius)
	{
		const float PI = 3.14159265;
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float angle = (r1 * 2.0f - 1.0f) * PI;
		float radius = minRadius + sqrt(r2 * 30.0f);
		return { radius * cos(angle), radius * sin(angle), 0.0f };
	}


	glm::vec3 randomPosInCircle(float maxRadius)
	{
		const float PI = 3.14159265;
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float angle = (r1 * 2.0f - 1.0f) * PI;
		float radius = sqrt(r2) * maxRadius * 0.9; 
		return { radius * cos(angle), radius * sin(angle), 0.0f };
	}

	////////////////////////////////////////
	///	     		prefabs				////
	////////////////////////////////////////
	void createPlayer()
	{
		Team team = Team::NONE;
		int ID = handleManager->NewHandle();
		transformSystem->AddComponent(ID, { 0.0f, 0.0f, 5.0f }, { 20.0f, 20.0f, 0.0f });
		cameraSystem->AddComponent(ID);
		userInputSystem->AddComponent(ID);
		graphicsSystem->AddComponent(ID, shaderProgram, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/Circle.png").ID, SolengineV2::Colour(0, 255, 0, 255));
		colliderSystem->AddComponent(ID, ColliderType::CIRCULAR);

		int childID = handleManager->NewHandle();
		transformSystem->AddChild(ID, childID, glm::vec3{ 0.0f, -15.0f, 0.0f }, glm::vec3{ 18.0f, 3.0f, 10.0f });
		graphicsSystem->AddComponent(childID, shaderProgram, GraphicsType::WORLDUI, resourceManager->GetTexture("Textures/blank.png").ID, SolengineV2::Colour(0, 150, 0, 155));
		healthSystem->AddComponent(ID, childID);

		int UIID = handleManager->NewHandle();
		transformSystem->AddComponent(UIID, { 140.0f, 40.0f, 5.0f }, { 100.0f, 100.0f, 0.0f });
		graphicsSystem->AddComponent(UIID, shaderProgram, GraphicsType::UI, resourceManager->GetTexture("Textures/blank.png").ID, SolengineV2::Colour(222, 150, 0, 255));
	}

	void createFood(glm::vec3 pos)
	{
		int ID = handleManager->NewHandle();
		Team team = Team::ONE;
		transformSystem->AddComponent(ID, pos, { 10.0f, 10.0f, 0.0f });
		graphicsSystem->AddComponent(ID, shaderProgram, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/bul.png").ID, SolengineV2::Colour(255, 150, 0, 255));
		colliderSystem->AddComponent(ID, ColliderType::FOOD);
		selectableSystem->AddComponent(ID);
		foodSystem->AddComponent(ID);
		//cameraSystem->AddComponent(ID);

		int childID = handleManager->NewHandle();
		transformSystem->AddChild(ID, childID, glm::vec3{ 0.0f, -15.0f, 0.0f }, glm::vec3{ 18.0f, 3.0f, 10.0f });
		graphicsSystem->AddComponent(childID, shaderProgram, GraphicsType::WORLDUI, resourceManager->GetTexture("Textures/blank.png").ID, SolengineV2::Colour(0, 150, 0, 155));
		healthSystem->AddComponent(ID, childID);

		//piManager->AddComponent();
	}

	void createUnit(glm::vec3 pos)
	{
		int ID = handleManager->NewHandle();
		Team team = Team::ONE;
		transformSystem->AddComponent(ID, pos, { 20.0f, 20.0f, 0.0f });
		graphicsSystem->AddComponent(ID, shaderProgram, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/Circle.png").ID, SolengineV2::Colour(255, 255, 255, 255));
		colliderSystem->AddComponent(ID, ColliderType::CIRCULAR);
		velocitySystem->AddComponent(ID, 0, { 0.0f, 0.0f });
		survivalSystem->AddComponent(ID);
	}

	void createFloor(glm::vec3 pos)
	{
		int ID = handleManager->NewHandle();
		transformSystem->AddComponent(ID, pos, { 800.0f, 800.0f, -10.0f });
		graphicsSystem->AddComponent(ID, shaderProgram, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/background1.png").ID, SolengineV2::Colour(255, 255, 255, 100));
	}
};