#pragma once
#include <vector>

class DataComponent
{
	template <typename T> friend class DataSystemInterface;
	std::vector<float> points{ 0.5f };
};
