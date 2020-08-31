#pragma once
#include <random>

namespace SolengineV2
{
	class RandomNumberGenerator
	{
	public:
		RandomNumberGenerator() 
		{
			std::random_device rd{};
			mt = std::mt19937{ rd() };
		}
		std::mt19937 mt{};
		std::uniform_int_distribution<int> dist{ 0, std::numeric_limits<int>::max() };

		float getRandom(const float min, const float max)
		{
			return min + getRandomZeroToOne() * (max - min);
		}

		float getRandomZeroToOne()
		{
			return static_cast<float>(dist(mt)) / static_cast<float>(std::numeric_limits<int>::max());
		}
	};
}