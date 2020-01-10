#pragma once

struct PlayerInputComponent
{
	PlayerInputComponent() : PlayerInputComponent(0) {}
	PlayerInputComponent(std::shared_ptr<TransformComponent> TC) : transformCo(TC) {}

	std::shared_ptr<TransformComponent> transformCo;
};
