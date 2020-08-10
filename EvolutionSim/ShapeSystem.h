#pragma once
#include <ShaderProgram.h>

struct CameraManager;
struct SelectionBox;

class ShapeSystem
{
public:
	void update(const SelectionBox& selectionBox, const CameraManager& camera, const SolengineV2::ShaderProgram& shaderProgram) const;
};