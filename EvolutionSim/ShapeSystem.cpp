#include <GeometryRenderer.h>
#include <ShaderManager.h>
#include "GameData.h"
#include "ShapeSystemSpecialization.h"
#include "CameraSystemSpecialization.h"
#include "TransformSystemSpecialization.h"

void ShapeSystem::update(const SolengineV2::ShaderProgram& shaderProgram, const GameData& gameData) const
{
	SolengineV2::ShaderManager::use(shaderProgram, nullptr);
	SolengineV2::ShaderManager::setProjectionMatrix(shaderProgram, Camera::System::getProjectionMatrix(gameData.camera.cam));
	if (Transform::System::getPos(gameData.selectionBox.transform).x < FLT_MAX)
	{
		SolengineV2::GeometryRenderer::draw(
			shaderProgram.programID, 
			Shape::System::getShape(gameData.selectionBox.shape), 
			Transform::System::getPos(gameData.selectionBox.transform), 
			Transform::System::getDims(gameData.selectionBox.transform)
		);
	}
	SolengineV2::ShaderManager::unuse(shaderProgram);
}