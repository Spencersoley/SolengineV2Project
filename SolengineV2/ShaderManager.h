#pragma once
#include <string>
#include <GL/glew.h>
#include <vector>
#include <fstream>

#include "IOManager.h"
#include "ShaderProgram.h"

namespace SolengineV2
{
	class ShaderManager
	{
	public:
		static GLuint getUniformLocation(const ShaderProgram& shader, const std::string& uniformName) 
		{
			GLint location = glGetUniformLocation(shader.programID, uniformName.c_str());
			if (location == GL_INVALID_INDEX) throw std::invalid_argument("Uniform " + uniformName + " not found in shader!");
			return location;
		}

		static void use(const ShaderProgram& shader, const char* uniform)
		{
			glUseProgram(shader.programID);
			for (int i = 0; i < shader.numAttributes; i++)
			{
				glEnableVertexAttribArray(i);
			}

			if (uniform)
			{
				GLint textureUniform = getUniformLocation(shader, uniform);
				glUniform1i(textureUniform, 0);
			}
		}

		static void setProjectionMatrix(const ShaderProgram& shader, const glm::mat4& projection) 
		{
			glUniformMatrix4fv(getUniformLocation(shader, "P"), 1, GL_FALSE, &(projection[0][0]));
		}

		static void unuse(const ShaderProgram& sp)
		{
			glUseProgram(0);
			for (int i = 0; i < sp.numAttributes; i++)
			{
				glDisableVertexAttribArray(i);
			}
		}
	};
}
