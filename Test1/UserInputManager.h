#pragma once
#include <InputManager.h>
#include "TransformManager.h"
#include "CameraManager.h"

struct UserInputComponent
{
	UserInputComponent(std::shared_ptr<TransformComponent> TC) : transformCo(TC) {}

	std::shared_ptr<TransformComponent> transformCo;
};

class UserInputManager
{
	SolengineV2::InputManager* IM;
	TransformManager* TM;
	CameraManager* CM;

public:
	UserInputManager(TransformManager* transManager, CameraManager* camManager, SolengineV2::InputManager* inputManager) 
		: TM(transManager), CM(camManager), IM(inputManager) {} 

	std::vector<UserInputComponent> PlayerInputs{};

	std::vector<glm::vec2> newpos;

	bool shouldGenerate = false; // for testing purposes

	void AddComponent()
	{
		PlayerInputs.push_back(UserInputComponent(TM->GetLastTransform()));
	}

	void Process(int dt)
	{
		for (auto it = PlayerInputs.begin(); it != PlayerInputs.end();)
		{
			TransformComponent* TC = it->transformCo.get();
			if (TM->IsToBeDeleted(TC))
			{
				it = PlayerInputs.erase(it);
			}
			else
			{
				++it; 

				if (IM->KeyState(SDLK_a))
				{
					TM->Translate(TC, { -0.1f * dt, 0.0f });
				}
				if (IM->KeyState(SDLK_d))
				{
					TM->Translate(TC, { 0.1f * dt, 0.0f });
				}
				if (IM->KeyState(SDLK_w))
				{
					TM->Translate(TC, { 0.0f, 0.1f * dt });
				}
				if (IM->KeyState(SDLK_s))
				{
					TM->Translate(TC, { 0.0f, -0.1f * dt });
				}
			}
		}

		if (IM->KeyState(SDLK_q))
		{
			CM->Zoom(0.01f * dt);
		}
		if (IM->KeyState(SDLK_e))
		{
			CM->Zoom(-0.01f * dt);
		}

		if (IM->KeyPress(SDLK_x))
		{
			shouldGenerate = true;
		}
	}
};
