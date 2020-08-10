#pragma once
#include <climits>

class SelectedComponent
{
	using Handle = unsigned int;
	template <class T> friend class SelectableSystemInterface;
	Handle handle{ UINT_MAX };
};