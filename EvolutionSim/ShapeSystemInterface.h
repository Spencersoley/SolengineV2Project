#pragma once
#include "ShapeComponent.h"
namespace SolengineV2
{
	struct Square;
	class ShaderManager;
	struct ShaderProgram;
}
struct GameData;

template <class Implementation>
class ShapeSystemInterface
{
public:
	static const SolengineV2::Square& getShape(const ShapeComponent& shape) { return shape.square; }

	static const void initSquare(ShapeComponent& shape) { shape.square.Init(); }

	static void update(const SolengineV2::ShaderProgram& shaderProgram, const GameData& gameData)
	{
		static Implementation system;
		system.update(shaderProgram, gameData);
	};
};