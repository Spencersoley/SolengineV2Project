#pragma once
#include <string>
#include <GL/glew.h>
#include <vector>
#include <fstream>

#include "IOManager.h"
#include <glm\ext\matrix_float4x4.hpp>
#include <map>

namespace SolengineV2
{
	class ShaderProgram
	{
	public:
		ShaderProgram() : numAttributes(0), programID(0), vertexShaderID(0), fragmentShaderID(0) {}
	
		int numAttributes;
		GLuint programID;
		GLuint vertexShaderID;
		GLuint fragmentShaderID;
	};

	class ShaderProgramManager
	{
	public:
		std::map<int, ShaderProgram> shaderMap;

		void SetProjectionMatrix(const ShaderProgram* sp, glm::mat4 projection)
		{
			Use(sp);
			glUniformMatrix4fv(GetUniformLocation(sp, "P"), 1, GL_FALSE, &(projection[0][0]));
		}

		GLuint GetUniformLocation(const ShaderProgram* sp, const std::string& uniformName)
		{
			GLint location = glGetUniformLocation(sp->programID, uniformName.c_str());
			if (location == GL_INVALID_INDEX)
			{
				std::cout << "Uniform " + uniformName + " not found in shader!";
			}
			return location;
		}

		void Use(const ShaderProgram* sp)
		{
			glUseProgram(sp->programID);
			for (int i = 0; i < sp->numAttributes; i++)
				glEnableVertexAttribArray(i);

			//DrawCode, optional? Tells it to use texture unit 0
			//glActiveTexture(GL_TEXTURE0);
			//Shader uses texture 0
			GLint textureUniform = GetUniformLocation(sp, "mySampler");
			glUniform1i(textureUniform, 0);
		}

		void Unuse(const ShaderProgram* sp)
		{
			glUseProgram(0);
			for (int i = 0; i < sp->numAttributes; i++)
				glDisableVertexAttribArray(i);
		}
	};


}
