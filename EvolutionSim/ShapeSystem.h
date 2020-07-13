#include "ShaderProgram.h"
#include "ShaderManager.h"
#include <GeometryRenderer.h>

class TransformSystem;
class CameraSystem;
class SelectableSystem;
class BeingManager;

struct ShapeComponent
{
	SolengineV2::Square shape;
};

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

	void process(const BeingManager& beings) const;

private:
	const SolengineV2::GeometryRenderer& renderer;
	const SolengineV2::ShaderProgram& shader;
	const SolengineV2::ShaderManager& shaderManager;

	const TransformSystem& transformSystem;
	const CameraSystem& cameraSystem;
	const SelectableSystem& selectableSystem;

	struct
	{
		ShapeComponent selectionBox;
	} components;
};