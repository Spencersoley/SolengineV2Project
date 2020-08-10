#include "BeingManager.h"
#include "SelectionBox.h"
#include "SelectedTracker.h"

#include "SelectableSystemImplementation.h"
#include "TransformSystemImplementation.h"

using Handle = unsigned int;

void SelectableSystem::processClick(const glm::vec2& mouseCoords, BeingManager& beings, SelectedTracker& selected) const
{
	float selectionProximity = FLT_MAX;
	Handle tempSelected{ UINT_MAX };

	size_t beingsSize = beings.pool.size();
	for (Handle beingHandle = 0; beingHandle < beingsSize; ++beingHandle)
	{
		const glm::vec2& beingPos = Transform::System::getPos(beings.pool[beingHandle].transform);
		const glm::vec2& beingDims = Transform::System::getDims(beings.pool[beingHandle].transform);
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

	Selectable::System::setHandle(selected.component, tempSelected);
}

void SelectableSystem::update(const BeingManager& beings, SelectionBox& selectionBox, SelectedTracker& selected) const
{
	Handle selectedHandle = Selectable::System::getHandle(selected.component);
	if (selectedHandle < beings.getSize()) // is valid handle
	{
		Transform::System::setPos(selectionBox.transform, Transform::System::getPos(beings.pool[selectedHandle].transform));
	}
	else //if invalid handle
	{
		Transform::System::setPos(selectionBox.transform, glm::vec2(FLT_MAX));
	}
}

void SelectableSystem::clearSelectedHandle(SelectedComponent& component) const
{
	Selectable::System::setHandle(component, UINT_MAX); 
}