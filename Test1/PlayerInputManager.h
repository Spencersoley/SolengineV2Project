#pragma once
#include <InputManager.h>
#include "TransformManager.h"
#include "CameraManager.h"
#include "PlayerInputComponent.h"

class PlayerInputManager
{
public:
	PlayerInputManager(TransformManager* transManager, CameraManager* camManager) //, EntityCreator* eCreator)
		: TM(transManager), CM(camManager) {} //, EC(eCreator) {}

	SolengineV2::InputManager IM;
	TransformManager* TM;
	CameraManager* CM;

	bool shouldGenerate = false;

	std::vector<PlayerInputComponent> PlayerInputs{};

	void Process(int dt)
	{
		IM.ProcessInput();

		for (auto it = PlayerInputs.begin(); it != PlayerInputs.end();)
		{
			TransformComponent* TC = it->transformCo.get();
			if (TM->IsToBeDeleted(TC))
			{
				std::cout << "PIM----------USE COUNT: " << it->transformCo.use_count() << std::endl;
				it = PlayerInputs.erase(it);
				std::cout << "PlayerInputs size: " << PlayerInputs.size() << std::endl;
			}
			else
			{
				++it; 

				if (IM.KeyState(SDLK_a))
				{
					TM->Translate(TC, { -0.1f * dt, 0.0f });
				}
				if (IM.KeyState(SDLK_d))
				{
					TM->Translate(TC, { 0.1f * dt, 0.0f });
				}
				if (IM.KeyState(SDLK_w))
				{
					TM->Translate(TC, { 0.0f, 0.1f * dt });
				}
				if (IM.KeyState(SDLK_s))
				{
					TM->Translate(TC, { 0.0f, -0.1f * dt });
				}
			}
		}

		if (IM.KeyState(SDLK_q))
		{
			CM->Zoom(0.01f * dt);
		}
		if (IM.KeyState(SDLK_e))
		{
			CM->Zoom(-0.01f * dt);
		}

		if (IM.KeyPress(SDLK_x))
		{
			shouldGenerate = true;
		}
	}

	void AddComponent()
	{
		PlayerInputs.push_back(TM->GetLastTransform());
	}
};
