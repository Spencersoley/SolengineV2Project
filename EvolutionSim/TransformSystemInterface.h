#pragma once
#include "TransformComponent.h"

template <class Implementation>
class TransformSystemInterface
{
public:
	static const glm::vec2& getPos(const TransformComponent& component) { return component.pos; }
	static const glm::vec2& getDims(const TransformComponent& component) { return component.dims; }

	static void setPos(TransformComponent& component, const glm::vec2& set) { component.pos = set; }
	static void setDims(TransformComponent& component, const glm::vec2& set) { component.dims = set; }

	static void translate(TransformComponent& component, const glm::vec2& transVec)
	{
		static Implementation system;
		system.translate(component, transVec);
	}
};