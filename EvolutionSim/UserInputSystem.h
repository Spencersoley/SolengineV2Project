#pragma once

namespace SolengineV2
{
	class InputManager;
	enum class GameState;
}
class BeingManager;
struct Camera;
class TransformSystem;
class CameraSystem;
class SelectableSystem;

class UserInputSystem
{
public:
	UserInputSystem(
		SolengineV2::InputManager& input,
		TransformSystem& tformSys,
		CameraSystem& camSys,
		SelectableSystem& selSys
	) :
		inputManager(input),
		transformSystem(tformSys),
		cameraSystem(camSys),
		selectableSystem(selSys)
	{}

	void update(const unsigned int dt, BeingManager& beingManager, Camera& camera, SolengineV2::GameState& gameState);

private:
	SolengineV2::InputManager& inputManager;

	TransformSystem& transformSystem;
	CameraSystem& cameraSystem;
	SelectableSystem& selectableSystem;
};
