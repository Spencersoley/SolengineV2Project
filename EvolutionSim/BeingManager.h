#pragma once
#include <vector>
#include "Vertex.h"
#include "Being.h"

class BeingManager
{
	using Handle = unsigned int;

public:
	size_t getSize() const;
	void init(const size_t size);
	void deleteBeing(const Handle handle, Handle& selectedHandle);
	void resize(const Handle start);
	void setToSize(const size_t size);
	void clear();
	void reset();

	std::vector<Being> pool{};
};