#pragma once
#include <glm\ext\vector_float2.hpp>

class TransformComponent;

class TransformSystem
{
public:
	const glm::vec2& getPos(const TransformComponent& component) const;
	const glm::vec2& getDims(const TransformComponent& component) const;

	void setPos(TransformComponent& component, const glm::vec2& set) const;
	void setDims(TransformComponent& component, const glm::vec2& set) const;

	void translate(TransformComponent& component, const glm::vec2& transVec) const;
private:
	float getDistFromCentre(const TransformComponent& component) const;
	float getDist(const glm::vec2& a, const glm::vec2& b) const;
};