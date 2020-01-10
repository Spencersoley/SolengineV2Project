#pragma once

struct ColliderComponent
{
	//ColliderComponent() : ColliderComponent(0) {}
	ColliderComponent(std::shared_ptr<TransformComponent> TC) : transformCo(TC) {}

	std::shared_ptr<TransformComponent> transformCo;
};
