#pragma once
#include <vector>

class DataComponent
{
	template <typename T> friend class DataSystemInterface;
	std::vector<float> points{ };
	float currentPoint{ 0.0f };
};