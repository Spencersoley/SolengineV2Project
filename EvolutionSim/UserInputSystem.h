#pragma once
#include <InputManager.h>
#include "TransformSystem.h"
#include "CameraSystem.h"
#include "HealthSystem.h"
#include "SpriteSystem.h"

class UserInputComponent
{
private:
	friend class UserInputSystem;
	UserInputComponent(std::shared_ptr<TransformComponent> transform) 
		: 
		Transform(transform) 
	{}

	std::shared_ptr<TransformComponent> Transform;
};

class UserInputSystem
{
	SolengineV2::InputManager* inputManager;

	TransformSystem* transformSystem;
	CameraSystem* cameraSystem;
	HealthSystem* healthSystem;
	SpriteSystem* spriteSystem;

	std::map<int, UserInputComponent> userInputMap{};

public:
	UserInputSystem(
		TransformSystem* _transformSystem, 
		CameraSystem* _cameraSystem, 
		HealthSystem* _healthSystem, 
		SpriteSystem* _spriteSystem, 
		SolengineV2::InputManager* _inputManager
	) 
		:
		transformSystem(_transformSystem),
		cameraSystem(_cameraSystem),
		healthSystem(_healthSystem),
		spriteSystem(_spriteSystem),
		inputManager(_inputManager)
	{}

	std::vector<glm::vec2> newpos;

	bool shouldGenerateFood = false; // for testing purposes
	bool shouldGenerateUnits = false;

	void AddComponent(int handle)
	{
		userInputMap.try_emplace(handle, UserInputComponent(transformSystem->GetLast()));
	}

	void Process(int dt) 
	{
		for (auto it = userInputMap.begin(); it != userInputMap.end(); ++it)
		{
			TransformComponent* TC = it->second.Transform.get();
			if (inputManager->KeyState(SDLK_a))
			{
				transformSystem->Translate(TC, { -0.1f * dt, 0.0f });
			}
			if (inputManager->KeyState(SDLK_d))
			{
				transformSystem->Translate(TC, { 0.1f * dt, 0.0f });
			}
			if (inputManager->KeyState(SDLK_w))
			{
				transformSystem->Translate(TC, { 0.0f, 0.1f * dt });
			}
			if (inputManager->KeyState(SDLK_s))
			{
				transformSystem->Translate(TC, { 0.0f, -0.1f * dt });
			}
		}

		if (inputManager->KeyState(SDLK_q))
		{
			cameraSystem->Zoom(0.01f * dt);
		}
		if (inputManager->KeyState(SDLK_e))
		{
			cameraSystem->Zoom(-0.01f * dt);
		}

		if (inputManager->KeyPress(SDLK_x))
		{
			shouldGenerateFood = true;
		}

		if (inputManager->KeyPress(SDLK_c))
		{
			shouldGenerateUnits = true;
		}

		if (inputManager->KeyPress(SDLK_TAB))
		{
			ToggleDisplay();
		}
	}

	bool healthVisible = true;
	void ToggleDisplay() 
	{
		int healthbarHandle;
		std::cout << "Toggle";
		if (healthVisible)
		{
			for (auto healthComponentIt = healthSystem->GetHealthMapBegin(); healthComponentIt != healthSystem->GetHealthMapEnd(); ++healthComponentIt)
			{
				healthbarHandle = healthSystem->GetHealthbarHandle(&(healthComponentIt->second));
				spriteSystem->SetVisibility(healthbarHandle, false);
			}
			healthVisible = false;
		}
		else
		{
			for (auto it = healthSystem->GetHealthMapBegin(); it != healthSystem->GetHealthMapEnd(); ++it)
			{
				spriteSystem->SetVisibility(healthSystem->GetHealthbarHandle(&(it->second)), true);
			}
			healthVisible = true;
		}
	}

	void DeleteComponent(int handle)
	{
		userInputMap.erase(handle);
	}
};
