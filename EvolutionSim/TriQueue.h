#pragma once
#include <array>

template <class T>
struct TriQueue
{
	std::array<T, 3> data{ 0, 0, 0 };
};