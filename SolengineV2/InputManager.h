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
		~InputManager() {};

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

		void SetMouseCoords(int x, int y)
		{
			mouseCoords.x = (float)x;
			mouseCoords.y = (float)y;
		}

		void SetMouseWheel(float y)
		{
			mouseWheel = (float)y;
		}

		//Returns the state of the key id in the current map
		bool KeyState(unsigned int keyID)
		{
			auto it = keyMap.find(keyID);
			return (it != keyMap.end()) ? it->second : false;
		}

		//Returns the state of the key id in the previous map
		bool PreviousKeyState(unsigned int keyID)
		{
			auto it = previousKeyMap.find(keyID);
			return (it != previousKeyMap.end()) ? it->second : false;
		}

		//Checks all queued events, changing their states accordingly
		SolengineV2::GameState ProcessInput()
		{
			SetMouseWheel(0);
			updatePreviousKeyMap();

			SDL_Event evnt;

			while (SDL_PollEvent(&evnt))
			{
				switch (evnt.type)
				{
				case SDL_QUIT:
					return SolengineV2::GameState::EXIT;
					break;
				case SDL_MOUSEMOTION:
					SetMouseCoords(evnt.motion.x, evnt.motion.y);
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
				}
			}

			return SolengineV2::GameState::PLAY;
		}

		glm::vec2 GetMouseCoords() { return mouseCoords; }
		float GetMouseWheel() { return mouseWheel; }

	private:
		std::unordered_map<unsigned int, bool> keyMap;
		std::unordered_map<unsigned int, bool> previousKeyMap;
		glm::vec2 mouseCoords;
		float mouseWheel;

		//Copies the previous key states into a new map
		void updatePreviousKeyMap()
		{
			for (auto& it : keyMap)
			{
				previousKeyMap[it.first] = it.second;
			}
		}
	};
}
