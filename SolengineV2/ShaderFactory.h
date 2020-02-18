#pragma once

#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include "ShaderProgram.h"
#include "ShaderManager.h"
#include "../deps/include/GL/glew.h"

namespace SolengineV2
{
	class ShaderFactory
	{
	public:
		ShaderFactory(ShaderManager* _shaderManager) : shaderManager(_shaderManager) {}

		void CreateShader(
			const std::string& shaderName,
			const std::string& vertexShaderFilePath,
			const std::string& fragmentShaderFilePath,
			const std::vector<std::string>& attributeNames)
		{
			//Creates shader IDs 
			GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			if (vertexShaderID == 0) throw std::invalid_argument("Vertex shader failed to be created!"); 
			GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			if (fragmentShaderID == 0) throw std::invalid_argument("Fragment shader failed to be created!"); 
			GLuint programID = glCreateProgram();

			//Compiles individual shaders
			compileShader(vertexShaderFilePath, vertexShaderID);
			compileShader(fragmentShaderFilePath, fragmentShaderID);

			//Create shader instance
			std::shared_ptr<ShaderProgram> shader(new ShaderProgram());
			shaderManager->AddShader(shaderName, shader);

			//Assign compiled IDs to instance
			shader->programID = programID;
			shader->vertexShaderID = vertexShaderID;
			shader->fragmentShaderID = fragmentShaderID;
			addAttribute(shader, attributeNames);
			linkShaders(shader);
		}

	private:
		IOManager iOManager;
		ShaderManager* shaderManager;

		//Parses a file and associates it with an ID
		void compileShader(const std::string& filePath, GLuint id)
		{
			//Extracts from file, to line, formatted into fileContents
			std::string fileContents = iOManager.ReadFileToStringBuffer(filePath);

			//Associates contents with shader id and compiles
			const char* contents = fileContents.c_str();
			glShaderSource(id, 1, &contents, nullptr);
			glCompileShader(id);

			GLint success = 0;
			glGetShaderiv(id, GL_COMPILE_STATUS, &success);

			if (success == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> errorLog(maxLength);
				glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

				glDeleteShader(id);

				std::printf("%s\n", (&errorLog[0]));
				throw std::invalid_argument("Shader " + filePath + "failed to compile");
			}
		}

		//Links a vertex shader and fragment shader into shader program by ID
		void linkShaders(const std::shared_ptr<ShaderProgram> shaderProg)
		{
			GLuint programID = shaderProg->programID;
			GLuint vertexShaderID = shaderProg->vertexShaderID;
			GLuint fragmentShaderID = shaderProg->fragmentShaderID;

			glAttachShader(programID, vertexShaderID);
			glAttachShader(programID, fragmentShaderID);

			glLinkProgram(programID);

			GLint isLinked = 0;
			glGetProgramiv(programID, GL_LINK_STATUS, (int*)&isLinked);

			if (isLinked == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> errorLog(maxLength);
				glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

				glDeleteProgram(programID);

				glDeleteShader(vertexShaderID);
				glDeleteShader(fragmentShaderID);

				std::printf("%s\n", &(errorLog[0]));
				throw std::invalid_argument("Shaders failed to link");
			}

			glDetachShader(programID, vertexShaderID);
			glDetachShader(programID, fragmentShaderID);
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
		}

		void addAttribute(std::shared_ptr<ShaderProgram> shaderProg, const std::vector<std::string>& attributeName)
		{
			for (size_t i = 0; i < attributeName.size(); i++)
				glBindAttribLocation(shaderProg->programID, shaderProg->numAttributes++, attributeName[i].c_str());
		}
	};


}