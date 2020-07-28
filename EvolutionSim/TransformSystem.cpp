#include "TransformSystem.h"
#include <algorithm>
#include "TransformComponent.h"

const glm::vec2& TransformSystem::getPos(const TransformComponent& component) const
{
	return component.pos;
}

const glm::vec2& TransformSystem::getDims(const TransformComponent& component) const
{
	return component.dims;
}

void TransformSystem::setPos(TransformComponent& component, const glm::vec2& set) const
{
	component.pos = set;
}

void TransformSystem::setDims(TransformComponent& component, const glm::vec2& set) const
{
	component.dims = set;
}

void TransformSystem::translate(TransformComponent& component, const glm::vec2& transVec) const
{
	component.pos += transVec;
}

float TransformSystem::getDistFromCentre(const TransformComponent& component) const
{
	return getDist(getPos(component), glm::vec2(0.0f));
}

float TransformSystem::getDist(const glm::vec2& a, const glm::vec2& b) const
{
	return static_cast<float>(sqrt(static_cast<double>(a.x) * static_cast<double>(b.x)) + static_cast<double>(a.y) * static_cast<double>(b.y));
}