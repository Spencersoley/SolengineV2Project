#pragma once
#include <vector>
#include <set>

template <typename T>
struct MemoryPool
{
	// allocated space for pool
	uint32_t space{ 0 };
	// fixed size component vector
	std::vector<T> data{};

	void init(uint32_t prefabLimit);
	void insert(const uint32_t beingHandle, const T& comp);
	void deleteComponent(uint32_t handle);
};

template<typename T>
inline void MemoryPool<T>::init(const uint32_t prefabLimit)
{
	space = prefabLimit;
	uint32_t size = prefabLimit * sizeof(T);
	data.reserve(size);
}

template<typename T>
inline void MemoryPool<T>::insert(const uint32_t beingHandle, const T& comp)
{
	if (space == data.size()) return; // max capacity
	data[beingHandle] = comp;
}

template<typename T>
inline void MemoryPool<T>::deleteComponent(uint32_t handle)
{
	if (data.size() == 0) return;
	data[handle] = data.back();
	data.pop_back();
}

