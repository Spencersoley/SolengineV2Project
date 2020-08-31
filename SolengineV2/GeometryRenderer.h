#pragma once
#include <SDL/SDL.h>
#include <vector>
#include <GL/glew.h>
#include <glm\ext\vector_float2.hpp>

namespace SolengineV2
{
	struct Square
	{
		std::vector<uint32_t> indices =
		{ 
			0, 1, 2, 3
		};

		unsigned int VBO{}, VAO{}, EBO{};

		void Init()
		{
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glEnableVertexAttribArray(0);
		}
	};

	struct Circle
	{

	};

	class GeometryRenderer
	{
	public:
		static void draw(const GLuint programID, const Square& square, const glm::vec2& pos, const glm::vec2& dims) 
		{
			glBindVertexArray(0);
			glUseProgram(programID);

			glBindVertexArray(square.VAO);
			glBindBuffer(GL_ARRAY_BUFFER, square.VBO);

			std::vector<float> vertices =
			{
				pos.x - dims.x / 2.0f, pos.y - dims.y / 2.0f, 0.0f,
			    pos.x + dims.x / 2.0f, pos.y - dims.y / 2.0f, 0.0f,
				pos.x + dims.x / 2.0f, pos.y + dims.y / 2.0f, 0.0f,
			    pos.x - dims.x / 2.0f, pos.y + dims.y / 2.0f, 0.0f
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * square.indices.size(), square.indices.data(), GL_DYNAMIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

			glBindVertexArray(square.VAO); 
			glDrawElements(GL_LINE_LOOP, square.indices.size(), GL_UNSIGNED_INT, 0);
		}
				
		static void draw(const glm::vec2& pos, const glm::vec2& dims, unsigned int numberOfSegments) 
		{
			glBegin(GL_LINE_LOOP);
			for (unsigned int ii = 0; ii < numberOfSegments; ii++) 
			{
				float theta = 2.0f * 3.1415926f * float(ii) / float(numberOfSegments);//get the current angle 
				float x = dims.x * cosf(theta);//calculate the x component 
				float y = dims.y * sinf(theta);//calculate the y component 
				glVertex2f(x + pos.x, y + pos.y);//output vertex 
			}
			glEnd();
		}
	};
}
