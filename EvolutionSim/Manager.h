#pragma once
#include <vector>

template <class Subject>
class Manager
{
	using Handle = unsigned int;
public:
	std::vector<Subject> pool{};

	size_t getSize() const
	{
		return pool.size();
	}

	void init(const size_t size)
	{
		if (size == 0) return;
		pool.reserve(size * sizeof(Subject));
	}

	void deleteByHandle(const Handle handle, Handle& selectedHandle)
	{
		pool[handle] = pool.back();
		pool.pop_back();
	}

	void resize(const Handle start)
	{
		pool.resize(start);
	}

	void setToSize(const size_t size)
	{
		pool.resize(size);
	}

	void clear()
	{
		pool.clear();
	}

	void reset()
	{
		uint32_t size = getSize();
		clear();
		init(size);
	}
};