#pragma once
#include <SDL/SDL.h>

namespace SolengineV2
{
	struct Square
	{
		Square()
		{
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glEnableVertexAttribArray(0);
		}

		std::vector<uint32_t> indices =
		{ 
			0, 1, 2, 3
		};

		unsigned int VBO, VAO, EBO;
	};

	class GeometryRenderer
	{
	public:
		void draw(const GLuint programID, const Square& square, const glm::vec2& pos, const glm::vec2& dims) const
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
	};
}
