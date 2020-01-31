#pragma once
#include <vector>
#include "TransformSystem.h"
#include "CameraSystem.h"
#include <InputManager.h>

class SelectableComponent
{
private:
	friend class SelectableSystem;
	SelectableComponent(std::shared_ptr<TransformComponent> transform) : Transform(transform) {}

	std::shared_ptr<TransformComponent> Transform;
};

class SelectableSystem : public System
{
	TransformSystem* TS;
	CameraSystem* CS;
	SolengineV2::InputManager* IM;

public:
	SelectableSystem(TransformSystem* ts, CameraSystem* cs, SolengineV2::InputManager* im) : TS(ts), CS(cs), IM(im) {}
	std::map<int, SelectableComponent> Selectables{};

	void AddComponent(int handle)
	{
		Selectables.insert(std::pair<int, SelectableComponent>(handle, SelectableComponent(TS->GetLast())));
	}

	void Process()
	{
		if (IM->KeyState(SDL_BUTTON_LEFT))
		{
			bool ctrlDown = IM->KeyState(SDLK_LCTRL);
			for (auto& selectable : Selectables)
			{
				TransformComponent* TC = selectable.second.Transform.get();
				glm::vec3 pos = TS->GetPos(TC);
				glm::vec3 dims = TS->GetDims(TC);
				float xDist = dims.x / 2.0f;
				float yDist = dims.y / 2.0f;
				glm::vec2 mouseCoords = CS->ScreenToWorldConvert(IM->GetMouseCoords());

				if (mouseCoords.x < pos.x + xDist
					&& mouseCoords.x > pos.x - xDist
					&& mouseCoords.y < pos.y + yDist
					&& mouseCoords.y > pos.y - yDist)
				{
					TS->SetIsSelected(TC, true);
				}
				else 
				{
					if (!ctrlDown)
					{
						TS->SetIsSelected(TC, false);
					}
				}
			}
		}
	}

	void DeleteComponent(int handle)
	{
		Selectables.erase(handle);
	}
};