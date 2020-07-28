#pragma once

namespace SolengineV2
{
	class GeometryRenderer;
	class ShaderManager;
	struct ShaderProgram;
}
class BeingManager;
struct SelectionBox;
struct Camera;
class TransformSystem;
class CameraSystem;
class SelectableSystem;

class ShapeSystem
{
public:
	ShapeSystem(
		const SolengineV2::GeometryRenderer& geometryRenderer,
		const SolengineV2::ShaderManager& manager,
		const SolengineV2::ShaderProgram& shaderProgram,
		const TransformSystem& tformSys,
		const CameraSystem& camSys,
		const SelectableSystem& selSys
	) :
		renderer(geometryRenderer), 
		shaderManager(manager),
		shader(shaderProgram),
		transformSystem(tformSys),
		cameraSystem(camSys),
		selectableSystem(selSys)
	{}

	void update(const SelectionBox& selectionBox, const Camera& camera) const;

private:
	const SolengineV2::GeometryRenderer& renderer;
	const SolengineV2::ShaderProgram& shader;
	const SolengineV2::ShaderManager& shaderManager;

	const TransformSystem& transformSystem;
	const CameraSystem& cameraSystem;
	const SelectableSystem& selectableSystem;
};