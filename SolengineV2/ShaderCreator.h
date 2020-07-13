#pragma once
#include <string>
#include <GL/glew.h>
#include <vector>
#include <fstream>

#include "IOManager.h"
#include "ShaderProgram.h"

namespace SolengineV2
{
	class ShaderCreator
	{
	public:
		ShaderCreator(IOManager& io) : ioManager(io) {}

		void createShader(
			ShaderProgram& shader,
			const std::string& vertexShaderFilePath,
			const std::string& fragmentShaderFilePath,
			const std::vector<std::string>& attributeNames
		)
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

			//Assign compiled IDs to instance
			shader.programID = programID;
			shader.vertexShaderID = vertexShaderID;
			shader.fragmentShaderID = fragmentShaderID;
			addAttribute(shader, attributeNames);
			linkShaders(shader);
		}

	private:
		IOManager& ioManager;

		//Parses a file and associates it with an ID
		void compileShader(const std::string& filePath, const GLuint id) const
		{
			//Extracts from file, to line, formatted into fileContents
			std::string fileContents = ioManager.readFileToStringBuffer(filePath);

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
		void linkShaders(const ShaderProgram& shader)
		{
			GLuint programID = shader.programID;
			GLuint vertexShaderID = shader.vertexShaderID;
			GLuint fragmentShaderID = shader.fragmentShaderID;

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

		void addAttribute(ShaderProgram& shader, const std::vector<std::string>& attributeName)
		{
			for (uint32_t i = 0; i < attributeName.size(); i++)
			{
				glBindAttribLocation(shader.programID, shader.numAttributes++, attributeName[i].c_str());

			}
		}
	};
}
