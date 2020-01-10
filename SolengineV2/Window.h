#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>
#include <vertex.h>

namespace SolengineV2 
{
    enum WindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4 };

    class Window
    {
    public:
        Window() : sdlWindow(nullptr), screenHeight(0), screenWidth(0) {};
        Window(std::string windowName, int sw, int sh, unsigned int currentFlags, Colour col) : Window()
        {
            Init(windowName, sw, sh, currentFlags, col);
        }
        ~Window() {};

        int Init(std::string windowName, int sw, int sh, unsigned int currentFlags, Colour col)
        {
            Uint32 flags = SDL_WINDOW_OPENGL;
            screenWidth = sw;
            screenHeight = sh;

            if (currentFlags& INVISIBLE) 
            {
                flags |= SDL_WINDOW_HIDDEN;
            }
            if (currentFlags& FULLSCREEN) 
            {
                flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
            }
            if (currentFlags& BORDERLESS) 
            {
                flags |= SDL_WINDOW_BORDERLESS;
            }

            //Open an SDL window
            sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sw, sh, flags);
            if (sdlWindow == nullptr) 
            {
                std::cout << "SDL Window could not be created!";
            }

            //Set up our OpenGL context
            SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
            if (glContext == nullptr) 
            {
                std::cout << "SDL_GL context could not be created!";
            }

            //Set up glew (optional but recommended)
            GLenum error = glewInit();
            if (error != GLEW_OK) 
            {
                std::cout << "Could not initialize glew!";
            }

            //Check the OpenGL version
            std::cout << "***   OpenGL Version: %s   ***" << std::endl<< glGetString(GL_VERSION);

            glClearColor((float)col.r/255, (float)col.g/(float)255, (float)col.b/(float)255, (float)col.a/(float)255);

            //Set VSYNC
            SDL_GL_SetSwapInterval(0);

            // Enable alpha blend
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            return 0;
        };

        void SwapBuffer()
        {
            SDL_GL_SwapWindow(sdlWindow);
        }

        int GetScreenWidth() { return screenWidth; }
        int GetScreenHeight() { return screenHeight; }

    private:
        SDL_Window* sdlWindow = nullptr;
        int screenWidth, screenHeight;
    };
}
