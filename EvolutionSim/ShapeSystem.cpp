#include <GeometryRenderer.h>
#include <ShaderManager.h>

#include "SelectionBox.h"
#include "CameraManager.h"

#include "ShapeSystemImplementation.h"
#include "CameraSystemImplementation.h"
#include "TransformSystemImplementation.h"

void ShapeSystem::update(const SelectionBox& selectionBox, const CameraManager& camera, const SolengineV2::ShaderProgram& shaderProgram) const
{
	SolengineV2::ShaderManager::use(shaderProgram, nullptr);
	SolengineV2::ShaderManager::setProjectionMatrix(shaderProgram, Camera::System::getProjectionMatrix(camera.cam));
	if (Transform::System::getPos(selectionBox.transform).x < FLT_MAX)
	{
		SolengineV2::GeometryRenderer::draw(shaderProgram.programID, Shape::System::getShape(selectionBox.shape), Transform::System::getPos(selectionBox.transform), Transform::System::getDims(selectionBox.transform));
	}
	SolengineV2::ShaderManager::unuse(shaderProgram);
}
