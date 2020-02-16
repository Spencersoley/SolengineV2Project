#pragma once
#include <vector>

class HandleManager
{
public:
	int NewHandle()
	{
		if (!availableHandles.empty())
		{
			int handle = availableHandles[availableHandles.size() - 1];
			availableHandles.pop_back();
			return handle;
		}
		else
		{
			int handle = ++handleCounter;
			return handle;
		}
	};

	void DeleteHandle(int handle)
	{
		availableHandles.push_back(handle);
	}

private:
	int handleCounter = 0;
	std::vector<int> availableHandles;
};