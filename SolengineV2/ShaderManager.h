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
		friend class ShaderFactory;

		void AddShader(std::string shaderName, std::shared_ptr<ShaderProgram> shader)
		{
			shaderMap.insert_or_assign(shaderName, shader);
		}

		std::map<const std::string, std::shared_ptr<ShaderProgram>> shaderMap;

	public:	
		std::shared_ptr<ShaderProgram> GetShader(const std::string& shaderName)
		{
			auto it = shaderMap.find(shaderName);
			if (it == shaderMap.end()) throw std::invalid_argument("Shader name does not exist in shader map");
			return it->second;
		}

		GLuint GetUniformLocation(const std::shared_ptr<ShaderProgram> sp, const std::string& uniformName)
		{
			GLint location = glGetUniformLocation(sp->programID, uniformName.c_str());
			if (location == GL_INVALID_INDEX) throw std::invalid_argument("Uniform " + uniformName + " not found in shader!");
			return location;
		}

		void Use(const std::shared_ptr<ShaderProgram> sp)
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

		void SetProjectionMatrix(const std::shared_ptr<ShaderProgram> sp, glm::mat4 projection)
		{
			Use(sp);
			glUniformMatrix4fv(GetUniformLocation(sp, "P"), 1, GL_FALSE, &(projection[0][0]));
		}

		void Unuse(const std::shared_ptr<ShaderProgram> sp)
		{
			glUseProgram(0);
			for (int i = 0; i < sp->numAttributes; i++)
				glDisableVertexAttribArray(i);
		}
	};
}
