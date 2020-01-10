#pragma once
#include <string>
#include <GL/glew.h>
#include <vector>
#include <fstream>

#include "IOManager.h"

namespace SolengineV2
{
	class ShaderProgram
	{
	public:
		ShaderProgram() : numAttributes(0), programID(0), vertexShaderID(0), fragmentShaderID(0) {}
		~ShaderProgram() {}

		GLuint GetUniformLocation(const std::string& uniformName)
		{
			GLint location = glGetUniformLocation(programID, uniformName.c_str());
			if (location == GL_INVALID_INDEX)
				std::cout << "Uniform " + uniformName + " not found in shader!";

			return location;
		}

		void Use()
		{
			//Set base depth
			glClearDepth(1.0);
			//Clear colour and depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(programID);
			for (int i = 0; i < numAttributes; i++)
				glEnableVertexAttribArray(i);

			//DrawCode, optional? Tells it to use texture unit 0
			glActiveTexture(GL_TEXTURE0);
			//Shader uses texture 0
			GLint textureUniform = GetUniformLocation("mySampler");
			glUniform1i(textureUniform, 0);
		}

		void Unuse()
		{
			glUseProgram(0);
			for (int i = 0; i < numAttributes; i++)
				glDisableVertexAttribArray(i);
		}

		int numAttributes;
		GLuint programID;
		GLuint vertexShaderID;
		GLuint fragmentShaderID;
	};
}
