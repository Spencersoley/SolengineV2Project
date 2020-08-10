#pragma once
#include "ShapeComponent.h"

namespace SolengineV2
{
	class Square;
	class ShaderManager;
	class ShaderProgram;
}

class SelectionBox;
class CameraManager;
template <class Implementation>
class ShapeSystemInterface
{
public:
	static const SolengineV2::Square& getShape(const ShapeComponent& shape) { return shape.square; }

	static void update(const SelectionBox& selectionBox, const CameraManager& camera, const SolengineV2::ShaderProgram& shaderProgram)
	{
		static Implementation system;
		system.update(selectionBox, camera, shaderProgram);
	};
};
