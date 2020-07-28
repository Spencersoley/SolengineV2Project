#pragma once
#include <vector>

class DataComponent
{
	friend class DataSystem;
	std::vector<float> points{ 0.5f };
};
