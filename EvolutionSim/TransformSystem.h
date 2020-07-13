#pragma once
#include <vector>
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
#include <algorithm>
#include <memory>
#include "MemoryPool.h"
#include <array>

#include "TransformComponent.h"

constexpr float ARENA_SIZE = 1600.0f;

class TransformSystem
{
public:
	enum class Transform
	{
		CAMERA,
		BACKGROUND,
		SELECTION_BOX,
		ARRAY_SIZE
	};

	const glm::vec2& getPos  (const TransformComponent& component) const { return component.pos;  }
	const glm::vec2& getDims (const TransformComponent& component) const { return component.dims; }

	const glm::vec2& getCameraPos()            const { return getPos  (getCComponent(Transform::CAMERA        )); }
	const glm::vec2& getBackgroundPos()        const { return getPos  (getCComponent(Transform::BACKGROUND    )); }
	const glm::vec2& getBackgroundDims()       const { return getDims (getCComponent(Transform::BACKGROUND    )); }
	const glm::vec2& getSelectionBoxPos()      const { return getPos  (getCComponent(Transform::SELECTION_BOX )); }
	const glm::vec2& getSelectionBoxDims()     const { return getDims (getCComponent(Transform::SELECTION_BOX )); }

	void setSelectionBoxPos(const glm::vec2& pos) { setPos(getComponent(Transform::SELECTION_BOX), pos); }
	void translate(TransformComponent& component, const glm::vec2& transVec) const { component.pos += transVec; }
	void translateCamera(const glm::vec2& transVec) { translate(getComponent(Transform::CAMERA), transVec); }

private:
	TransformComponent& getComponent(Transform handle) { return components[static_cast<int>(handle)]; }
	const TransformComponent& getCComponent(Transform handle) const { return components[static_cast<int>(handle)]; }
	void setPos(TransformComponent& component, const glm::vec2& pos) const { component.pos = pos; }

	float getDistFromCentre(const TransformComponent& component) const { return getDist(getPos(component), glm::vec2(0.0f)); }
	float getDist(const glm::vec2& a, const glm::vec2& b) const { return static_cast<float>(sqrt(static_cast<double>(a.x) * static_cast<double>(b.x)) + static_cast<double>(a.y) * static_cast<double>(b.y)); }

	std::array<TransformComponent, static_cast<int>(Transform::ARRAY_SIZE)> components
	{ 
		TransformComponent(glm::vec2(0.0f),  glm::vec2(0.0f)),            //CAMERA
		TransformComponent(glm::vec2(0.0f),  glm::vec2(ARENA_SIZE)),      //BACKGROUND
		TransformComponent(glm::vec2(50.0f), glm::vec2(30.0f))            //SELECITONBOX
	};
};