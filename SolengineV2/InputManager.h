#pragma once
#include <SDL/SDL.h>
#include <unordered_map>
#include <glm/glm.hpp>

#include "GameState.h"

namespace SolengineV2
{
	//Checks for inputs, changes keystates
	class InputManager
	{
	public:
		InputManager() : mouseCoords(0), mouseWheel(0) {};

		//Using SDL keyIDs for now, would be better to wrap keyIDs in your own enum
		//Changes the state of the key in the current map
		void KeyDown(unsigned int keyID)
		{
			//Creates a pair if it doesn't exist in the map
			keyMap[keyID] = true;
		}

		//Changes the state of the key in the current map
		void KeyUp(unsigned int keyID)
		{
			keyMap[keyID] = false;
		}

		//checks for a key press
		bool KeyPress(unsigned int keyID)
		{
			return (KeyState(keyID) && !PreviousKeyState(keyID)) ? true : false;
		}

		void UpdateMousePosition()
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			mouseCoords.x = x;
			mouseCoords.y = y;
		}

		void SetMouseCoords(int x, int y)
		{
			mouseCoords.x += x;
			mouseCoords.y += y;
		}

		void SetMouseWheel(int y)
		{
			mouseWheel = y;
		}

		//Returns the state of the key id in the current map
		bool KeyState(unsigned int keyID) const
		{
			const auto it = keyMap.find(keyID);
			return (it != keyMap.end()) ? it->second : false;
		}

		//Returns the state of the key id in the previous map
		bool PreviousKeyState(unsigned int keyID) const
		{
			const auto it = previousKeyMap.find(keyID);
			return (it != previousKeyMap.end()) ? it->second : false;
		}

		//Checks all queued events, changing their states accordingly
		void ProcessInput(SolengineV2::GameState& state)
		{
			SetMouseWheel(0);
			updatePreviousFrame();

			SDL_Event evnt;

			while (SDL_PollEvent(&evnt))
			{
				switch (evnt.type)
				{
				case SDL_QUIT:
					state = SolengineV2::GameState::EXIT;
					break;
				case SDL_MOUSEMOTION:
					UpdateMousePosition();
					//SetMouseCoords(evnt.motion.xrel, evnt.motion.yrel);
					break;
				case SDL_MOUSEBUTTONDOWN:
					KeyDown(evnt.button.button);
					break;
				case SDL_MOUSEBUTTONUP:
					KeyUp(evnt.button.button);
					break;
				case SDL_MOUSEWHEEL:
					SetMouseWheel(evnt.wheel.y);
					break;
				case SDL_KEYDOWN:
					KeyDown(evnt.key.keysym.sym);
					break;
				case SDL_KEYUP:
					KeyUp(evnt.key.keysym.sym);
					break;
					//case SDL_QUIT:
                   //case : ... if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
				}
			}
		}

		glm::ivec2 GetMouseCoords() const { return mouseCoords; }
		glm::ivec2 GetMouseOffset() const { return mouseCoords - previousMouseCoords; }
		int GetMouseWheel() const { return mouseWheel; }
		int GetMouseWheelOffset() const { return mouseWheel - previousMouseWheel; }

	private:

		std::unordered_map<unsigned int, bool> keyMap;
		std::unordered_map<unsigned int, bool> previousKeyMap;
		glm::ivec2 mouseCoords;
		glm::ivec2 previousMouseCoords{ 0, 0};
		int mouseWheel, previousMouseWheel{ 0 };

		//Copies the previous key states into a new map
		void updatePreviousFrame()
		{
			for (auto& it : keyMap)
			{
				previousKeyMap[it.first] = it.second;
			}

			previousMouseCoords = mouseCoords;
			previousMouseWheel = mouseWheel;
		}
	};
}
