#pragma once
#include <chrono>
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <algorithm>

constexpr uint32_t SAMPLES = 10;

namespace SolengineV2
{
	enum class TimerMode { NONE, SAMPLE_AVERAGE, LIFETIME_AVERAGE };

	class TimeManager
	{
	private:
		std::chrono::milliseconds desiredTicksPerFrame{};
		std::chrono::steady_clock::time_point endTime{};
		std::chrono::steady_clock::time_point startTime{};
		std::chrono::steady_clock::time_point previousStartTime{};

		uint32_t microsPerFrame[SAMPLES]{};
		uint32_t frameCount{ 0 };
		TimerMode mode{ TimerMode::NONE };


	public:
		TimeManager(uint32_t maxFPS, TimerMode timerMode) 
		{
			Init(maxFPS, timerMode);
		}
		TimeManager() {};

		void Init(uint32_t maxFPS, TimerMode timerMode)
		{
			mode = timerMode;
			desiredTicksPerFrame = std::chrono::milliseconds(1000 / maxFPS);
			previousStartTime = std::chrono::high_resolution_clock::now();
			endTime = std::chrono::high_resolution_clock::now();
		}

		void limitFPS()
		{
			std::chrono::steady_clock::time_point endTime = now();
			uint32_t frameNaframeMicro = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count());
			std::chrono::milliseconds frameTicks = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::microseconds(frameNaframeMicro));
			//Prevents frame finishing earlier and FPS breaking max fps
			if (desiredTicksPerFrame > frameTicks)
			{
				SDL_Delay(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(desiredTicksPerFrame - frameTicks).count()));
			}

			if (mode == TimerMode::SAMPLE_AVERAGE)
			{
				reportFPS(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(now() - startTime).count()));
			}
			else if (mode == TimerMode::LIFETIME_AVERAGE)
			{
				collateFPS(static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(now() - startTime).count()));
			}
		}

		std::chrono::steady_clock::time_point now()
		{
			return std::chrono::high_resolution_clock::now();
		}

		//returns change in time in microseconds
		template <class T>
		auto getDeltaTime()
		{
			startTime = now();
		    auto microseconds = std::chrono::duration_cast<T>(startTime - previousStartTime);
			previousStartTime = startTime;
			return microseconds;
		}

	private:
		void collateFPS(uint32_t frameMicros)
		{
			microsPerFrame[frameCount % SAMPLES] = frameMicros;
			frameCount++;

			if (frameCount % SAMPLES == 0)
			{
				//Calculates new average frame time
				uint32_t frames = std::min(frameCount, SAMPLES);
				uint32_t microsPerFrameTotal = 0;
				for (uint32_t i = 0; i < frames; i++)
				{
					microsPerFrameTotal += microsPerFrame[i];
				}

				float nanosPerFrameAverage = static_cast<float>(microsPerFrameTotal) / frames;
				float FPS = 0.0f;
				if (nanosPerFrameAverage > 0.0f)
				{
					FPS = 1000000000 / nanosPerFrameAverage;
				}
				static long long fpsTotal{ 0 };
				fpsTotal += static_cast<long long>(FPS);
				std::cout << "Lifetime average: " << (fpsTotal * SAMPLES) / frameCount << '\n';
			}
		}

		//Announces FPS every NUM_SAMPLES frames
		void reportFPS(uint32_t frameMicros)
		{
			microsPerFrame[frameCount % SAMPLES] = frameMicros;
			frameCount++;

			//Announces fps every n cycles
			if (frameCount % SAMPLES == 0)
			{
				//Calculates new average frame time
				uint32_t frames = std::min(frameCount, SAMPLES);
				uint32_t nanosPerFrameTotal = 0;
				for (uint32_t i = 0; i < frames; i++)
				{
					nanosPerFrameTotal += microsPerFrame[i];
				}
				float nanosPerFrameAverage = static_cast<float>(nanosPerFrameTotal) / frames;
				float FPSTracked = 0.0f;
				if (nanosPerFrameAverage > 0.0f)
				{
					FPSTracked = 1000000 / nanosPerFrameAverage;
				}
				std::cout << "Sample average: " << FPSTracked << '\n';
			}
		}
	};
}
