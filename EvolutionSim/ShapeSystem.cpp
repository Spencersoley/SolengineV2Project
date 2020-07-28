#include "ShaderManager.h"
#include "GeometryRenderer.h"
#include "SelectionBox.h"
#include "Camera.h"
#include "CameraSystem.h"
#include "ShapeSystem.h"
#include "TransformSystem.h"

void ShapeSystem::update(const SelectionBox& selectionBox, const Camera& camera) const
{
	shaderManager.use(shader, nullptr);
	shaderManager.setProjectionMatrix(shader, cameraSystem.getProjectionMatrix(camera.cam));
	if (transformSystem.getPos(selectionBox.transform).x < FLT_MAX)
	{
		renderer.draw(shader.programID, selectionBox.shape.shape, transformSystem.getPos(selectionBox.transform), transformSystem.getDims(selectionBox.transform));
	}
	shaderManager.unuse(shader);
}
