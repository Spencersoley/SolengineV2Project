#pragma once

#include <IMGUI/imgui.h>
#include <numeric>

class SolengineV2::Window;
class VelocitySystem;
class GenerationSystem;
class DataSystem;
class SelectableSystem;
class SurvivalSystem;
class BeingManager;

class GUISystem
{
public:
	GUISystem(
		SolengineV2::Window& wndw,
		VelocitySystem& velSys,
		GenerationSystem& genSys,
		DataSystem& dataSys,
		const GeneSystem& geneSys,
		const SelectableSystem& selSys,
		const SurvivalSystem& survSys
	);

	void process(BeingManager& beings, SolengineV2::GameState& state);
private:
	ImGuiIO io{};
	SolengineV2::Window& window;

	VelocitySystem& velocitySystem;
	GenerationSystem& generationSystem;
	DataSystem& dataSystem;
	const GeneSystem& geneSystem;
	const SelectableSystem& selectableSystem;
	const SurvivalSystem& survivalSystem;
};