#pragma once
#include <vector>
#include "TransformSystem.h"
#include "CameraSystem.h"
#include <InputManager.h>

class SelectableComponent
{
private:
	friend class SelectableSystem;
	SelectableComponent(std::shared_ptr<TransformComponent> transform) 
		: 
		transform(transform) 
	{}

	std::shared_ptr<TransformComponent> transform;
};

class SelectableSystem
{
	SolengineV2::InputManager* inputManager;

	TransformSystem* transformSystem;
	CameraSystem* cameraSystem;

	std::map<int, SelectableComponent> selectableMap{};

public:
	SelectableSystem(TransformSystem* ts, CameraSystem* cs, SolengineV2::InputManager* im) : transformSystem(ts), cameraSystem(cs), inputManager(im) {}


	void AddComponent(int handle)
	{
		selectableMap.try_emplace(handle, SelectableComponent(transformSystem->GetLast()));
	}

	void Process()
	{
		if (inputManager->KeyState(SDL_BUTTON_LEFT))
		{
			bool ctrlDown = inputManager->KeyState(SDLK_LCTRL);
			for (auto& selectable : selectableMap)
			{
				TransformComponent* TC = selectable.second.transform.get();
				glm::vec3 pos = transformSystem->GetPos(TC);
				glm::vec3 dims = transformSystem->GetDims(TC);
				float xDist = dims.x / 2.0f;
				float yDist = dims.y / 2.0f;
				glm::vec2 mouseCoords = cameraSystem->ScreenToWorldConvert(inputManager->GetMouseCoords());

				if (mouseCoords.x < pos.x + xDist
					&& mouseCoords.x > pos.x - xDist
					&& mouseCoords.y < pos.y + yDist
					&& mouseCoords.y > pos.y - yDist)
				{
					transformSystem->SetIsSelected(TC, true);
				}
				else 
				{
					if (!ctrlDown)
					{
						transformSystem->SetIsSelected(TC, false);
					}
				}
			}
		}
	}

	void DeleteComponent(int handle)
	{
		selectableMap.erase(handle);
	}
};