#pragma once
#include <glm\glm.hpp>
#include "Component.h"
#include "Transform.h"
#include <vector>

namespace SolengineV2
{
	class Patrol : public Component
	{
	public:
		Patrol() : Patrol({ {} }) {}
		Patrol(std::vector<glm::vec2> patrolPoints) : transform(nullptr), path(patrolPoints), pathProgress(0)
		{
			Type = ComponentType::PATROL;
		}
		~Patrol() {}

		void Awake() {}
		void Init() {}
		void PhysicsUpdate(int adjustedDeltaTicks)
		{
			patrol(adjustedDeltaTicks);
		}
		void Update() {}
		void LateUpdate() {}

	private:
		Transform* transform;
		std::vector<glm::vec2> path;
		int pathProgress;

		void patrol(int dt)
		{
			if (transform == nullptr) //PlayerController component always searches for transform component
			{
				transform = static_cast<Transform*>(GetCoComponent(ComponentType::TRANSFORM));
				if (transform == nullptr) return;
			}

			if (path.size() == 0) return;

			if (pathProgress >= path.size())
			{
				pathProgress = 0;
			}
			pathProgress += seek(pathProgress, dt);
		}

		bool seek(int targetIndex, int dt)
		{
			glm::vec2 target = path[targetIndex];
			glm::vec2 currentPos{ transform->GetXYPos() };
			
			float arriveDist = 10.0f;
			bool arrived = (abs(target.x - currentPos.x) < arriveDist && abs(target.y - currentPos.y) < arriveDist);

			if (arrived) 
			{
				return 1;
			} 
			else
			{
				transform->Translate(0.1f * glm::normalize(target - currentPos) *(float)dt);
				return 0;
			}
		}
	};
}