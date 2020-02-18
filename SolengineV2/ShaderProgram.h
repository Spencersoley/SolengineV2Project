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
	struct ShaderProgram
	{
		int numAttributes{ 0 };
		GLuint programID{ 0 };
		GLuint vertexShaderID{ 0 };
		GLuint fragmentShaderID{ 0 };
	};
}
