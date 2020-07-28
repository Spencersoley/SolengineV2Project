#include "BeingManager.h"
#include "SelectionBox.h"
#include "SelectableSystem.h"
#include "TransformSystem.h"

void SelectableSystem::processClick(const glm::vec2& mouseCoords, BeingManager& beings)
{
	float selectionProximity = FLT_MAX;
	Handle tempSelected{ UINT_MAX };

	size_t beingsSize = beings.pool.size();
	for (Handle beingHandle = 0; beingHandle < beingsSize; ++beingHandle)
	{
		const glm::vec2& beingPos = transformSystem.getPos(beings.pool[beingHandle].transform);
		const glm::vec2& beingDims = transformSystem.getDims(beings.pool[beingHandle].transform);
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
				tempSelected = beingHandle;
			}
		}
	}
	
	selectedHandle = tempSelected;
}

void SelectableSystem::update(const BeingManager& beings, SelectionBox& selectionBox) const
{
	if (selectedHandle < beings.getSize()) // is valid handle
	{
		transformSystem.setPos(selectionBox.transform, transformSystem.getPos(beings.pool[selectedHandle].transform));
	}
	else //invalid handle
	{
		transformSystem.setPos(selectionBox.transform, glm::vec2(FLT_MAX));
	}
}
