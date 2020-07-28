#pragma once
#include <IMGUI/imgui.h>

namespace SolengineV2
{
	class Window;
	enum class GameState;
}
class TextureSystem;
class BeingManager;
class DataSystem;
class GenerationSystem;
class OverlaySystem;
class SelectableSystem;
class SurvivalSystem;
class VelocitySystem;

class GUISystem
{
public:
	GUISystem(
		SolengineV2::Window& wndw,
		const TextureSystem& textureSys,
		VelocitySystem& velSys,
		GenerationSystem& genSys,
		DataSystem& dataSys,
		OverlaySystem& overlaySys,
		const GeneSystem& geneSys,
		const SelectableSystem& selSys,
		const SurvivalSystem& survSys
	);

	void update(BeingManager& beings, SolengineV2::GameState& state, unsigned int deltaTime);

private:
	ImGuiIO io{};
	SolengineV2::Window& window;
	const TextureSystem& textureSystem;

	enum class Menu { SETTINGS, HELP, OVERLAY } menu = Menu::SETTINGS;

	VelocitySystem& velocitySystem;
	GenerationSystem& generationSystem;
	DataSystem& dataSystem;
	OverlaySystem& overlaySystem;
	const GeneSystem& geneSystem;
	const SelectableSystem& selectableSystem;
	const SurvivalSystem& survivalSystem;

	void resetGame(SolengineV2::GameState& state, BeingManager& beings);
};