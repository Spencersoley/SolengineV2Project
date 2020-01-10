#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>

namespace SolengineV2
{
	void initialiseSDL()
	{
		//Initialise SDL
		SDL_Init(SDL_INIT_EVERYTHING);
		//Tells SDL we want a double buffered window to prevent flickering
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}
}
