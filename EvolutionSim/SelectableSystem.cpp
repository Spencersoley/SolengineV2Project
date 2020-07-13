#include "SelectableSystem.h"
#include "TransformSystem.h"
#include "BeingManager.h"

void SelectableSystem::processClick(const glm::vec2& mouseCoords, BeingManager& beings)
{
	float selectionProximity = std::numeric_limits<float>::max();
	bool anySelection{ false };
	for (uint32_t beingHandle = 0; beingHandle < beings.getSize(); beingHandle++)
	{
		const TransformComponent& transform = beings.getTransformComponent(beingHandle);
		const glm::vec2& beingPos = transformSystem.getPos(transform);
		const glm::vec2& beingDims = transformSystem.getDims(transform);
		const float xRadius = beingDims.x / 2.0f;
		const float yRadius = beingDims.y / 2.0f;

		if (mouseCoords.x < beingPos.x + xRadius
			&& mouseCoords.x > beingPos.x - xRadius
			&& mouseCoords.y < beingPos.y + yRadius
			&& mouseCoords.y > beingPos.y - yRadius)
		{
			const float xDist = abs(mouseCoords.x - beingPos.x);
			const float yDist = abs(mouseCoords.y - beingPos.y);
			const float dist = xDist * xDist + yDist * yDist;
			if (dist < selectionProximity)
			{
				selectionProximity = dist;
				selectedHandle = beingHandle;
				anySelection = true;
			}
		}
	}

	if (!anySelection)
	{
		clearSelectedHandle();
	}
}

void SelectableSystem::process(const BeingManager& beings) const
{
	if (selectedHandle < beings.getSize()) // is valid handle
	{
		transformSystem.setSelectionBoxPos(transformSystem.getPos(beings.getTransformComponent(selectedHandle)));
	}
	else //invalid handle
	{
		transformSystem.setSelectionBoxPos(glm::vec2(FLT_MAX));
	}
}
