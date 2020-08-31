#pragma once
#include <ShaderProgram.h>
struct GameData;

class ShapeSystem
{
public:
	void update(const SolengineV2::ShaderProgram& shaderProgram, const GameData& gameData) const;
};