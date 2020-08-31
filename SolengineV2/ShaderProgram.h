#pragma once
#include <GL/glew.h>

namespace SolengineV2
{
	struct ShaderProgram
	{
		int numAttributes{ 0 };
		GLuint programID{ 0 };
		GLuint vertexShaderID{ 0 };
		GLuint fragmentShaderID{ 0 };
	};
}
