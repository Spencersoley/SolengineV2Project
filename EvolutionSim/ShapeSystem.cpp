#include "ShapeSystem.h"
#include "BeingManager.h"
#include "CameraSystem.h"
#include "TransformSystem.h"
#include "SelectableSystem.h"

void ShapeSystem::process(const BeingManager& beings) const
{
	shaderManager.use(shader, nullptr);
	shaderManager.setProjectionMatrix(shader, cameraSystem.getProjectionMatrix());
	if (selectableSystem.getCSelectedHandle() < beings.getSize())
	{
		renderer.draw(shader.programID, components.selectionBox.shape, transformSystem.getSelectionBoxPos(), transformSystem.getSelectionBoxDims());
	}
	shaderManager.unuse(shader);
}