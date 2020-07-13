#pragma once
#include <chrono>
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <algorithm>

#define SAMPLES 10

namespace SolengineV2
{
	class TimeManager
	{
	private:
		std::chrono::milliseconds desiredTicksPerFrame{};
		bool trackFPS;
		std::chrono::steady_clock::time_point endTime{};
		std::chrono::steady_clock::time_point startTime{};
		std::chrono::steady_clock::time_point previousStartTime{};

		long long nanosPerFrame[SAMPLES]{};
		long long frameCount = 0;

	public:
		TimeManager(unsigned int maxFPS, bool track)
			:
			trackFPS(track)
		{
			desiredTicksPerFrame = std::chrono::milliseconds(1000 / maxFPS);
			previousStartTime = std::chrono::high_resolution_clock::now();
			endTime = std::chrono::high_resolution_clock::now();
		}

		void limitFPS(bool track)
		{
			std::chrono::steady_clock::time_point endTime = now();
			long long frameNano = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
			std::chrono::milliseconds frameTicks = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds(frameNano));
			//Prevents frame finishing earlier and FPS breaking max fps
			if (desiredTicksPerFrame > frameTicks)
			{
				SDL_Delay(std::chrono::duration_cast<std::chrono::milliseconds>(desiredTicksPerFrame - frameTicks).count());
			}

			if (track)
			{
				//includes delay
				reportFPS(std::chrono::duration_cast<std::chrono::nanoseconds>(now() - startTime).count());
			}
		}

		std::chrono::steady_clock::time_point now()
		{
			return std::chrono::high_resolution_clock::now();
		}

		//returns change in time in nanoseconds
		long long getDeltaTime()
		{
			startTime = now();
			long long nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(startTime - previousStartTime).count();
			previousStartTime = startTime;
			return nanoSeconds;
		}

	private:

		//Announces FPS every NUM_SAMPLES frames
		void reportFPS(long long frameNS)
		{
			nanosPerFrame[frameCount % SAMPLES] = frameNS;
			frameCount++;

			//Announces fps every 10 cycles
			if (frameCount % 10 == 0)
			{
				//Calculates new average frame time
				uint32_t frames = std::min(frameCount, (long long)SAMPLES);
				long long nanosPerFrameTotal = 0;
				for (uint32_t i = 0; i < frames; i++)
				{
					nanosPerFrameTotal += nanosPerFrame[i];
				}
				double nanosPerFrameAverage = nanosPerFrameTotal / frames;
				double FPSTracked = 0.0f;
				if (nanosPerFrameAverage > 0.0f)
				{
					FPSTracked = 1000000000 / nanosPerFrameAverage;
				}
				std::cout << FPSTracked << '\n';
			}
		}
	};
}
