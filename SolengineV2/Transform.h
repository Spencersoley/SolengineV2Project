#pragma once
#include <glm\glm.hpp>
#include "Component.h"

namespace SolengineV2
{
	class Transform : public Component
	{
	public:
		Transform() : Transform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }) {}
		Transform(glm::vec2 pos, glm::vec2 dims) : Transform(glm::vec3{ pos.x, pos.y, 0.0f }, dims) {}
		Transform(glm::vec3 pos, glm::vec2 dims) : Pos(pos), Dims(dims), Updated(true)
		{
			Type = ComponentType::TRANSFORM;
		}
		~Transform() {}

		glm::vec3 Pos;
		glm::vec2 Dims;
		bool Updated;
		glm::vec2 GetXYPos() { return { Pos.x, Pos.y }; }

		void Awake() {}
		void Init() {}
		void PhysicsUpdate(int adj) {}
		void Update() {}
		void LateUpdate() 
		{
			Updated = false;
		}

		void Translate(glm::vec2 translate)
		{
			Pos.x += translate.x;
			Pos.y += translate.y;
			Updated = true;
		}
	};
}