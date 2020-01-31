#include <time.h>
#include "GraphicsSystem.h"
#include "CameraSystem.h"
#include "ColliderSystem.h"
#include "UserInputSystem.h"
#include "VelocitySystem.h"
#include "ShootSystem.h"
#include "LifetimeSystem.h"
#include "HandleManager.h"
#include "TargetableSystem.h"

class EntityDeleter
{
public:
	EntityDeleter() :
		transformSystem(nullptr),
		handleManager(nullptr),
		cameraSystem(nullptr),
		selectableSystem(nullptr),
		graphicsSystem(nullptr),
		userInputSystem(nullptr),
		colliderSystem(nullptr),
		velocitySystem(nullptr),
		shootSystem(nullptr),
		lifetimeSystem(nullptr),
		healthSystem(nullptr),
		targetableSystem(nullptr)
	{}

	TransformSystem* transformSystem;
	HandleManager* handleManager;
	CameraSystem* cameraSystem;
	SelectableSystem* selectableSystem;
	GraphicsSystem* graphicsSystem;
	UserInputSystem* userInputSystem;
	ColliderSystem* colliderSystem;
	VelocitySystem* velocitySystem;
	ShootSystem* shootSystem;
	LifetimeSystem* lifetimeSystem;
	HealthSystem* healthSystem;
	TargetableSystem* targetableSystem;

	void Init(TransformSystem* transform,
		HandleManager* handle,
		CameraSystem* camera,
		SelectableSystem* selectable,
		GraphicsSystem* graphics,
		UserInputSystem* userinput,
		ColliderSystem* collider,
		VelocitySystem* velocity,
		ShootSystem* shoot,
		LifetimeSystem* lifetime,
		HealthSystem* health,
		TargetableSystem* targetable)
	{
		transformSystem = transform;
		handleManager = handle;
		cameraSystem = camera;
		selectableSystem = selectable;
		graphicsSystem = graphics;
		userInputSystem = userinput;
		colliderSystem = collider;
		velocitySystem = velocity;
		shootSystem = shoot;
		lifetimeSystem = lifetime;
		healthSystem = health;
		targetableSystem = targetable;
	}

	void Process() //simple concept for generating new entities in game.
	{
		for (const int& handle : transformSystem->handlesToDelete)
		{
			//std::cout << "Transform size: " << transformManager->Transforms.size() << std::endl;
			handleManager->DeleteHandle(handle);

			cameraSystem->DeleteComponent(handle);
			graphicsSystem->DeleteComponent(handle);
			userInputSystem->DeleteComponent(handle);
			colliderSystem->DeleteComponent(handle);
			velocitySystem->DeleteComponent(handle);
			shootSystem->DeleteComponent(handle);
			lifetimeSystem->DeleteComponent(handle);
			healthSystem->DeleteComponent(handle);
			targetableSystem->DeleteComponent(handle);
			selectableSystem->DeleteComponent(handle);
		}

		transformSystem->handlesToDelete.clear();
	}
};