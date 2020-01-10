#pragma once
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <algorithm>

namespace SolengineV2
{
	class TimeManager
	{
	public:
		TimeManager() : TimeManager(60, false) {}
		TimeManager(unsigned int desiredFPS, bool track) : desiredTicks(1000 / (float)desiredFPS), trackFPS(track), startTicks(0) 
		{
			prevTicks = SDL_GetTicks();

		}
		~TimeManager() {}

		void LimitFPS(Uint32 desiredFrameTicks, bool track)
		{
			Uint32 frameTicks = SDL_GetTicks() - startTicks;

			//Prevents frame finishing earlier and FPS breaking max fps
			if (desiredFrameTicks > frameTicks)
			{
				SDL_Delay(desiredFrameTicks - frameTicks);
			}

			if (track)
			{
				reportFPS();
			}

			startTicks = SDL_GetTicks();
		}

		//Allow overloaded
		void LimitFPS() 
		{
			LimitFPS(desiredTicks, trackFPS);
		}

		void LimitFPS(bool track)
		{
			LimitFPS(desiredTicks, track);
		}

		Uint32 GetDeltaTicks()
		{
			Uint32 deltaTicks = SDL_GetTicks() - prevTicks;
			prevTicks = SDL_GetTicks();
			return deltaTicks;
		}

	private:
		Uint32 prevTicks;
		unsigned int startTicks;
		unsigned int desiredTicks;
		bool trackFPS;

		//Announces FPS every NUM_SAMPLES frames
		void reportFPS()
		{
			static const int NUM_SAMPLES = 10;
			static Uint32 ticksPerFrame[NUM_SAMPLES];
			static int frameCount = 0;


			static Uint32 currentTicks;
			static Uint32 prevTicks = SDL_GetTicks();

			float ticksPerFrameTotal;
			float ticksPerFrameAverage;
			float FPSTracked;

			//Grabs number of ticks(ms) passed
			currentTicks = SDL_GetTicks();

			//Stores up to 10 samples of milliseconds passed since the previous frame
			ticksPerFrame[frameCount % NUM_SAMPLES] = currentTicks - prevTicks;

			//Stores current milliseconds passed, for next fps calculation
			prevTicks = currentTicks;

			frameCount++;

			//Resets the average frame time
			ticksPerFrameTotal = 0;

			//Calculates new average frame time
			for (int i = 0; i < std::min(frameCount, NUM_SAMPLES); i++)
			{
				ticksPerFrameTotal += ticksPerFrame[i];
			}

			ticksPerFrameAverage = ticksPerFrameTotal / std::min(frameCount, NUM_SAMPLES);

			//Sets _fps based on average frame time
			if (ticksPerFrameAverage > 0.0f)
			{
				FPSTracked = 1000.0f / ticksPerFrameAverage;
			}
			else
			{
				FPSTracked = 0.0f;
			}

			//Announces fps every 10 cycles
			if (frameCount % 10 == 0)
			{
 				std::cout << FPSTracked << std::endl;
			}
		}
	};
}