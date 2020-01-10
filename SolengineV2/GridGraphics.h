#pragma once
#include "Sprite.h"
#include "Component.h"

namespace SolengineV2
{
	class GridGraphics : public Component
	{
	public:
		GridGraphics() {}
		~GridGraphics() {}

		void Awake() {}
		void Init() {}
		void PhysicsUpdate(int adj) {}
		void Update()
		{
		}
		void LateUpdate()
		{
			draw();
		}

	private:
		void draw()
		{
			glBegin(GL_LINES);
			glVertex2f(10, 10);
			glVertex2f(40, 20);
			glEnd();
		}

		Colour colour;
	};
}
