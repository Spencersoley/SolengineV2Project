#pragma once
#include <InputManager.h>
#include "TransformSystem.h"
#include "CameraSystem.h"

struct UserInputComponent
{
	UserInputComponent(std::shared_ptr<TransformComponent> transform) : Transform(transform) {}

	std::shared_ptr<TransformComponent> Transform;
};

class UserInputManager
{
	SolengineV2::InputManager* IM;
	TransformSystem* TS;
	CameraSystem* CS;

public:
	UserInputManager(TransformSystem* ts, CameraSystem* cs, SolengineV2::InputManager* im) :
		TS(ts), 
		CS(cs), 
		IM(im) {}

	std::map<int, UserInputComponent> UserInputs{};

	std::vector<glm::vec2> newpos;

	bool shouldGenerate = false; // for testing purposes

	void AddComponent(int handle)
	{
		UserInputs.insert(std::pair<int, UserInputComponent>(handle, UserInputComponent(TS->GetLast())));
	}

	void Process(int dt)
	{
		for (auto it = UserInputs.begin(); it != UserInputs.end(); ++it)
		{
			TransformComponent* TC = it->second.Transform.get();
			if (IM->KeyState(SDLK_a))
			{
				TS->Translate(TC, { -0.1f * dt, 0.0f });
			}
			if (IM->KeyState(SDLK_d))
			{
				TS->Translate(TC, { 0.1f * dt, 0.0f });
			}
			if (IM->KeyState(SDLK_w))
			{
				TS->Translate(TC, { 0.0f, 0.1f * dt });
			}
			if (IM->KeyState(SDLK_s))
			{
				TS->Translate(TC, { 0.0f, -0.1f * dt });
			}
		}

		if (IM->KeyState(SDLK_q))
		{
			CS->Zoom(0.01f * dt);
		}
		if (IM->KeyState(SDLK_e))
		{
			CS->Zoom(-0.01f * dt);
		}

		if (IM->KeyPress(SDLK_x))
		{
			shouldGenerate = true;
		}
	}

	void DeleteComponent(int handle)
	{
		UserInputs.erase(handle);
	}
};
