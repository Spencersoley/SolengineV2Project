#pragma once
#include <string>
#include <GL/glew.h>
#include <vector>
#include <fstream>

#include "IOManager.h"
#include "ShaderProgram.h"

namespace SolengineV2
{
	class ShaderCompiler
	{
	public:
		ShaderCompiler(IOManager* io) 
		{
			iOManager = io;
		}
		~ShaderCompiler() {}

		void CompileShaders(ShaderProgram& shaderProgram, const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, const std::vector<std::string>& attributeNames)
		{
			//Creates shader IDs 
			GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			if (vertexShaderID == 0) std::cout << "Vertex shader failed to be created!";

			GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			if (fragmentShaderID == 0) std::cout << "Fragment shader failed to be created!";

			GLuint programID = glCreateProgram();
			//Compiles individual shaders
			compileShader(vertexShaderFilePath, vertexShaderID);
			compileShader(fragmentShaderFilePath, fragmentShaderID);

			shaderProgram.programID = programID;
			shaderProgram.vertexShaderID = vertexShaderID;
			shaderProgram.fragmentShaderID = fragmentShaderID;

			addAttribute(shaderProgram, attributeNames);
			linkShaders(shaderProgram);
		}

	private:
		IOManager* iOManager;

		//Parses a file and associates it with an ID
		void compileShader(const std::string& filePath, GLuint id)
		{
			//Extracts from shareFile, to line, formatted into fileContents
			std::string fileContents = iOManager->ReadFileToStringBuffer(filePath);

			//Creates pointer to fileContents
			const char* contentsPtr = fileContents.c_str();
			glShaderSource(id, 1, &contentsPtr, nullptr);
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
				std::cout << "Shader " + filePath + "failed to compile";
			}
		}

		void addAttribute(ShaderProgram& shaderProg, const std::vector<std::string>& attributeName)
		{
			for (size_t i = 0; i < attributeName.size(); i++)
				glBindAttribLocation(shaderProg.programID, shaderProg.numAttributes++, attributeName[i].c_str());
		}

		void linkShaders(const ShaderProgram& shaderProg)
		{
			GLuint programID = shaderProg.programID;
			GLuint vertexShaderID = shaderProg.vertexShaderID;
			GLuint  fragmentShaderID = shaderProg.fragmentShaderID;

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
				std::cout << "Shaders failed to link";
			}

			glDetachShader(programID, vertexShaderID);
			glDetachShader(programID, fragmentShaderID);
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);
		}
	};
}
