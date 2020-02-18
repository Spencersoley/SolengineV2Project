#include <time.h>
#include <ResourceManager.h>
#include "GraphicsSystem.h"
#include "CameraSystem.h"
#include "ColliderSystem.h"
#include "UserInputSystem.h"
#include "VelocitySystem.h"
#include "TargetableSystem.h"
#include "HandleManager.h"
#include "SelectableSystem.h"
#include "DuplicationData.h"

// implement creator and deleter as a single class
class EntityManager
{
public:
	EntityManager()
		:
		defaultShader(nullptr),
		resourceManager(nullptr),
		transformSystem(nullptr),
		handleManager(nullptr),
		cameraSystem(nullptr),
		selectableSystem(nullptr),
		graphicsSystem(nullptr),
		userInputSystem(nullptr),
		colliderSystem(nullptr),
		velocitySystem(nullptr),
		healthSystem(nullptr),
		foodSystem(nullptr),
		survivalSystem(nullptr)
	{}

	void Init(std::shared_ptr<SolengineV2::ShaderProgram> _defaultShader,
		SolengineV2::ResourceManager* rm,
		TransformSystem* transform,
		HandleManager* handle,
		CameraSystem* camera,
		SelectableSystem* selectable,
		GraphicsSystem* graphics,
		UserInputSystem* userinput,
		ColliderSystem* collider,
		VelocitySystem* velocity,
		HealthSystem* health,
		FoodSystem* food,
		SurvivalSystem* survival
	)
	{
		defaultShader = _defaultShader;
		resourceManager = rm;

		transformSystem = transform;
		handleManager = handle;
		cameraSystem = camera;
		selectableSystem = selectable;
		graphicsSystem = graphics;
		userInputSystem = userinput;
		colliderSystem = collider;
		velocitySystem = velocity;
		healthSystem = health;
		foodSystem = food;
		survivalSystem = survival;

		srand(time(NULL));

		createFloor({ 0.0f, 0.0f, 0.0f });

		for (int i = 0; i < 5; i++)
		{
			createUnit(randomPosOutsideCircle(400.0f), {});
		}	
		
		generateFood(10);

		createPlayer();
	}

	void Process() //simple concept for generating new entities in game.
	{
		for (auto it = transformSystem->GetHandlesToDeleteBegin(); it != transformSystem->GetHandlesToDeleteEnd(); ++it)
		{
			int handle = (*it);
			handleManager->DeleteHandle(handle);

			graphicsSystem->DeleteComponent(handle);
			userInputSystem->DeleteComponent(handle);
			colliderSystem->DeleteComponent(handle);
			velocitySystem->DeleteComponent(handle);
			healthSystem->DeleteComponent(handle);
			selectableSystem->DeleteComponent(handle);
			foodSystem->DeleteComponent(handle);
			survivalSystem->DeleteComponent(handle);
		}

		transformSystem->ClearHandles();

		if (survivalSystem->DuplicationDataVec.size())
		{
			generateUnits(survivalSystem->DuplicationDataVec);
			generateFood(5);
			survivalSystem->DuplicationDataVec.clear();
		}
	}

private:
	std::shared_ptr<SolengineV2::ShaderProgram> defaultShader;
	SolengineV2::ResourceManager* resourceManager;

	TransformSystem* transformSystem;
	HandleManager* handleManager;
	CameraSystem* cameraSystem;
	SelectableSystem* selectableSystem;
	GraphicsSystem* graphicsSystem;
	UserInputSystem* userInputSystem;
	ColliderSystem* colliderSystem;
	VelocitySystem* velocitySystem;
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

	void generateUnits(const std::vector<DuplicationData>& dataVec) const
	{
		for (const auto& data : dataVec)
		{
			glm::vec3 newPos = data.pos;
			data.pos.x < 0 ? newPos.x += 0.01f : newPos.x -= 0.01f; // adjust
			data.pos.y < 0 ? newPos.y += 0.01f : newPos.y += 0.01f; // adjust
			createUnit(newPos, data.stats);
		}
	}

	void generateUnits(int num)
	{
		for (int i = 0; i < num; i++)
		{
			createUnit(randomPosOutsideCircle(400.0f), {});
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

	// STEPS FOR PREFAB CREATION
	// 0 - Create prefab function: void CreateX() { ... };, within which you put the following:
	// 1 - (VITAL) Create handle: int ID = handleManager->NewHandle();
	// 2 - (VITAL) Add transform component: transformSystem->AddComponent(ID, position, dimensions);
	// 3 - (OPTIONAL) Add desired components from system: 
	// eg, for control with user input : userInputSystem->AddComponent(ID);
	// or for a collider : colliderSystem->AddComponent(ID, ColliderType::CIRCULAR);
	// or for graphics   : AddComponent(UIID, shaderProgram, GraphicsType::UI, resourceManager->GetTexture("Textures/blank.png").ID, SolengineV2::Colour(222, 150, 0, 255));
	// 4 - (OPTIONAL) Add another prefab as a child to this prefab, with the potential for shared components a
	// call the function to instantiate a the new entity with the determined characteristics
	
	
	void createPlayer() const
	{
		int ID = handleManager->NewHandle();
		transformSystem->AddComponent(ID, { 0.0f, 0.0f, 5.0f }, { 20.0f, 20.0f, 0.0f });
		cameraSystem->SetCamera();
		userInputSystem->AddComponent(ID);
		//graphicsSystem->AddComponent(ID, shaderProgram, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/Circle.png").ID, SolengineV2::Colour(0, 255, 0, 255));
		//colliderSystem->AddComponent(ID, ColliderType::CIRCULAR);

		//int childID = handleManager->NewHandle();
		//transformSystem->AddChild(ID, childID, glm::vec3{ 0.0f, -15.0f, 0.0f }, glm::vec3{ 18.0f, 3.0f, 10.0f });
		//graphicsSystem->AddComponent(childID, shaderProgram, GraphicsType::WORLDUI, resourceManager->GetTexture("Textures/blank.png").ID, SolengineV2::Colour(0, 150, 0, 155));
		//healthSystem->AddComponent(ID, childID);

		//int UIID = handleManager->NewHandle();
		//transformSystem->AddComponent(UIID, { 140.0f, 40.0f, 5.0f }, { 100.0f, 100.0f, 0.0f });
		//graphicsSystem->AddComponent(UIID, shaderProgram, GraphicsType::UI, resourceManager->GetTexture("Textures/blank.png").ID, SolengineV2::Colour(222, 150, 0, 255));
	}

	void createFood(const glm::vec3& pos) const
	{
		int ID = handleManager->NewHandle();
		transformSystem->AddComponent(ID, pos, { 10.0f, 10.0f, 0.0f });
		graphicsSystem->AddComponent(ID, defaultShader, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/bul.png").ID, SolengineV2::Colour(255, 150, 0, 255));
		colliderSystem->AddComponent(ID, ColliderType::FOOD);
		selectableSystem->AddComponent(ID);
		foodSystem->AddComponent(ID);

		//int childID = handleManager->NewHandle();
		//transformSystem->AddChild(ID, childID, glm::vec3{ 0.0f, -15.0f, 0.0f }, glm::vec3{ 18.0f, 3.0f, 10.0f });
		//graphicsSystem->AddComponent(childID, shaderProgram, GraphicsType::WORLDUI, resourceManager->GetTexture("Textures/blank.png").ID, SolengineV2::Colour(0, 150, 0, 155));
		//healthSystem->AddComponent(ID, childID);

		//piManager->AddComponent();
	}

	void createUnit(const glm::vec3& pos, const StatSheet& stats) const
	{
		int ID = handleManager->NewHandle();
		transformSystem->AddComponent(ID, pos, { 20.0f, 20.0f, 0.0f });
		graphicsSystem->AddComponent(ID, defaultShader, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/Circle.png").ID, SolengineV2::Colour(255, 255, 255, 255));
		colliderSystem->AddComponent(ID, ColliderType::CIRCULAR);
		velocitySystem->AddComponent(ID, 0, { 0.0f, 0.0f });
		survivalSystem->AddComponent(ID, stats);
		selectableSystem->AddComponent(ID);
	}

	void createFloor(const glm::vec3& pos)
	{
		int ID = handleManager->NewHandle();
		transformSystem->AddComponent(ID, pos, { 800.0f, 800.0f, -10.0f });
		graphicsSystem->AddComponent(ID, defaultShader, GraphicsType::GENERAL, resourceManager->GetTexture("Textures/background1.png").ID, SolengineV2::Colour(255, 255, 255, 100));
	}
};